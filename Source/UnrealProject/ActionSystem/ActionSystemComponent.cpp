// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystemComponent.h"

#include "Action.h"
#include "ActionEffect.h"
#include "AttributeSet.h"
#include "SharedGameplayTags.h"


// Sets default values for this component's properties
UActionSystemComponent::UActionSystemComponent()
{
	bWantsInitializeComponent = true;
}

FAttribute* UActionSystemComponent::GetAttribute(FGameplayTag InAttributeTag) const
{
	FAttribute* Attribute = *AttributesByTag.Find(InAttributeTag);
	if (!Attribute)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute %s not found"), *InAttributeTag.ToString());
		return nullptr;
	}
	
	return Attribute;
}

float UActionSystemComponent::GetAttributeValue(FGameplayTag InAttributeTag) const
{
	FAttribute* Attribute = GetAttribute(InAttributeTag);
	if (!Attribute)
	{
		return 0.0f;
	}
	
	return Attribute->GetValue();
}

void UActionSystemComponent::AddDynamicAttributeChangedDelegate(const FGameplayTag InAttributeTag, const FOnAttributeDynamicChanged& InDelegate)
{
	TArray<FOnAttributeDynamicChanged>& Delegates = AttributeDynamicChangedDelegates.FindOrAdd(InAttributeTag);
	Delegates.Add(InDelegate);
}

void UActionSystemComponent::RemoveDynamicAttributeChangedDelegate(const FOnAttributeDynamicChanged& InDelegate)
{
	for (TTuple<FGameplayTag, TArray<FOnAttributeDynamicChanged>>& Delegate : AttributeDynamicChangedDelegates)
	{
		if (Delegate.Value.RemoveSingle(InDelegate) > 0)
		{
			break;
		}
	}
}

void UActionSystemComponent::RemoveAction(FGameplayTag ActionNameToRemove)
{
	TObjectPtr<UAction>* Action = Actions.Find(ActionNameToRemove);
	if (Action)
	{
		if ((*Action)->IsRunning)
		{
			(*Action)->StopAction();
		}
		
		int32 NumRemoved = Actions.Remove(ActionNameToRemove);
		ensure(NumRemoved == 1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Action %s not found"), *ActionNameToRemove.ToString());
	}
}

void UActionSystemComponent::AppendActiveTags(FGameplayTagContainer TagsToAppend)
{
	ActiveTags.AppendTags(TagsToAppend);
	
	CheckAgainstBlockedTags(TagsToAppend);
	
	for (auto Tag : TagsToAppend)
	{
		OnGameplayTagCountChanged.Broadcast(Tag, 1);
	}
}

void UActionSystemComponent::RemoveActiveTags(FGameplayTagContainer TagsToRemove)
{
	int32 PreviousCount = ActiveTags.Num();
	
	ActiveTags.RemoveTags(TagsToRemove);
	
	ensure((PreviousCount - ActiveTags.Num()) == TagsToRemove.Num());
	
	for (auto Tag : TagsToRemove)
	{
		OnGameplayTagCountChanged.Broadcast(Tag, 0);
	}
}

void UActionSystemComponent::GrantAction(TSubclassOf<UAction> ActionClass)
{
	const bool bIsEffectClass = ActionClass->IsChildOf(UActionEffect::StaticClass());
	if (bIsEffectClass)
	{
		for (const TTuple<FGameplayTag, TObjectPtr<UAction>> Tuple : Actions)
		{
			UActionEffect* Effect = Cast<UActionEffect>(Tuple.Value);
			if (Effect)
			{
				if (Effect->GetClass() == ActionClass)
				{
					Effect->IncrementStackSize();
					return;
				}
			}
		}
	}
	
	UAction* NewAction = NewObject<UAction>(this, ActionClass);
	Actions.Add(NewAction->GetActionName(), NewAction);
			
	if (bIsEffectClass)
	{
		ensure(NewAction->CanStart());
		NewAction->StartAction();
	}
}

void UActionSystemComponent::CheckAgainstBlockedTags(const FGameplayTagContainer& TagsToCheck)
{
	for (const TTuple<FGameplayTag, TObjectPtr<UAction>> Tuple : Actions)
	{
		if (Tuple.Value->IsRunning && TagsToCheck.HasAny(Tuple.Value->GetBlockedTags()))
		{
			Tuple.Value->StopAction();
		}
	}
}

void UActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (Attributes == nullptr)
	{
		Attributes = NewObject<UAttributeSet>(this, UAttributeSet::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("No default AttributeSet defined for %s, creating default AttributeSet."), *GetNameSafe(GetOwner()));
	}
	
	for (TFieldIterator<FStructProperty> It(Attributes->GetClass()); It; ++It)
	{
		FAttribute* FoundAttribute = It->ContainerPtrToValuePtr<FAttribute>(Attributes);

		FName Name = FName("Attribute." + It->GetName());
		FGameplayTag Tag = FGameplayTag::RequestGameplayTag(Name);
		AttributesByTag.Add(Tag, FoundAttribute);
	}
	
	for (const TSubclassOf<UAction>& ActionClass : DefaultActions)
	{
		if (ActionClass)
		{
			GrantAction(ActionClass);
		}
	}
}

void UActionSystemComponent::SetDefaultAttributes(TSubclassOf<UAttributeSet> InAttributeSetClass)
{
	check(!HasBeenInitialized());
	
	FObjectInitializer& ObjectInitializer = FObjectInitializer::Get();
	Attributes = Cast<UAttributeSet>(ObjectInitializer.CreateDefaultSubobject(this, TEXT("Attributes"), InAttributeSetClass, InAttributeSetClass));
}

void UActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (Attributes)
	{
		Attributes->InitializeAttributes();
	}
}

void UActionSystemComponent::ApplyAttributeChange(FGameplayTag InAttributeTag, float Delta, EAttributeModifyType ModifyType /*= Base*/)
{
	FAttribute* Attribute = GetAttribute(InAttributeTag);
	if (Attribute)
	{
		float OldValue = Attribute->GetValue();
		
		switch (ModifyType) {
			case Base:
				Attribute->Base += Delta;
				break;
			case Modifier:
				Attribute->Modifier += Delta;
				break;
			case OverrideBase:
				Attribute->Base = Delta;
				break;
			default:
				check(false);
		}
		
		Attributes->PostProcessAttributeChange();
		
		if (const FOnAttributeChanged* Delegate = AttributeChangedDelegates.Find(InAttributeTag))
		{
			Delegate->Broadcast(InAttributeTag, Attribute->GetValue(), OldValue);
		}

		TArray<FOnAttributeDynamicChanged>* DynamicDelegates = AttributeDynamicChangedDelegates.Find(InAttributeTag);
		if (DynamicDelegates)
		{
			for (int i = DynamicDelegates->Num() - 1; i >= 0; --i)
			{
				FOnAttributeDynamicChanged& Delegate = (*DynamicDelegates)[i];
				if (!Delegate.ExecuteIfBound(InAttributeTag, Attribute->GetValue(), OldValue))
				{
					UE_LOG(LogTemp, Warning, TEXT("Dynamic delegate for attribute %s failed to execute, deleting delegate for %s"), *InAttributeTag.ToString(), *GetNameSafe(GetOwner()));
					DynamicDelegates->RemoveAt(i);
				}
			}
		}
		
		float NewValue = Attribute->GetValue();
		if (!FMath::IsNearlyEqual(NewValue, OldValue))
		{
			UE_LOG(LogTemp, Warning, TEXT("Attribute %s changed from %f to %f"), *InAttributeTag.ToString(), OldValue, NewValue);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute %s not found"), *InAttributeTag.ToString());
	}
}

void UActionSystemComponent::StartAction(FGameplayTag InActionName)
{
	TObjectPtr<UAction>* Action = Actions.Find(InActionName);
	if (Action)
	{
		if ((*Action)->CanStart())
		{
			(*Action)->StartAction();
		}
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Action %s not found"), *InActionName.ToString());
}

void UActionSystemComponent::StopAction(FGameplayTag InActionName)
{
	TObjectPtr<UAction>* Action = Actions.Find(InActionName);
	if (Action)
	{
		if ((*Action)->IsRunning)
		{
			(*Action)->StopAction();
		}
		
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Action %s not found"), *InActionName.ToString());
}

bool UActionSystemComponent::HasEnoughAttributes(const TMap<FGameplayTag, float>& GameplayTags)
{
	float TotalToSpend = 0.0f;
	float TotalCost = 0.0f;

	if (GameplayTags.IsEmpty())
	{
		return true;
	}
	
	for (const TPair<FGameplayTag, float>& AttributeCost : GameplayTags)
	{
		FAttribute* Attribute = GetAttribute(AttributeCost.Key);
		if (!Attribute)
		{
			continue;
		}
		
		TotalToSpend += Attribute->GetValue();
		TotalCost += AttributeCost.Value;
	}
	
	return TotalToSpend >= TotalCost;
}
