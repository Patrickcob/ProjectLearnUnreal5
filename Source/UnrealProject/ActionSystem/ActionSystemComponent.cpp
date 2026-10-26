// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystemComponent.h"

#include "Action.h"
#include "AttributeSet.h"
#include "SharedGameplayTags.h"


// Sets default values for this component's properties
UActionSystemComponent::UActionSystemComponent()
{
	bWantsInitializeComponent = true;
	
	AttributeSetClass = UAttributeSet::StaticClass();
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

void UActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	Attributes = NewObject<UAttributeSet>(this, AttributeSetClass);

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
			UAction* NewAction = NewObject<UAction>(this, ActionClass);
			Actions.Add(NewAction->GetActionName(), NewAction);
		}
	}
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
		(*Action)->StopAction();
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
		if (!Attribute || Attribute->GetValue() < AttributeCost.Value)
		{
			continue;
		}
		
		TotalToSpend += Attribute->GetValue();
		TotalCost += AttributeCost.Value;
	}
	
	return TotalToSpend >= TotalCost;
}
