// Fill out your copyright notice in the Description page of Project Settings.


#include "Action.h"
#include "Logging/StructuredLog.h"
#include "ActionSystemComponent.h"

UActionSystemComponent* UAction::GetOwningComponent() const
{
	return Cast<UActionSystemComponent>(GetOuter());
}

bool UAction::CanStart() const
{
	return !GetOwningComponent()->GetActiveTags().HasAny(BlockedTags) 
	&& !IsRunning 
	&& FMath::IsNearlyZero(CoolDownTimeRemaining())
	&& GetOwningComponent()->HasEnoughAttributes(AttributesConsumedOnStart);
}

void UAction::StopAction_Implementation()
{
	auto GameTime = GetWorld()->GetTime();
	
	UE_LOGFMT(LogTemp, Log, "[{GameTime}]Action {ActionName} stopped.", GameTime.GetWorldTimeSeconds(), ActionName.ToString());
	
	CoolDownUntil = GetWorld()->GetTimeSeconds() + CoolDownTime;
	
	GetOwningComponent()->RemoveActiveTags(GrantsTags);
	
	IsRunning = false;
}

void UAction::StartAction_Implementation()
{
	auto GameTime = GetWorld()->GetTime();
	
	GetOwningComponent()->AppendActiveTags(GrantsTags);

	// Consume attributes
	for (const TPair<FGameplayTag, float>& AttributeCost : AttributesConsumedOnStart)
	{
		FAttribute* Attribute = GetOwningComponent()->GetAttribute(AttributeCost.Key);
		if (Attribute && Attribute->GetValue() >= AttributeCost.Value)
		{
			// Consume the attribute
			GetOwningComponent()->ApplyAttributeChange(AttributeCost.Key, -AttributeCost.Value, Base);
			UE_LOG(LogTemp, Log, TEXT("Consumed %f of attribute %s"), AttributeCost.Value, *AttributeCost.Key.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough of attribute %s to start action %s"), *AttributeCost.Key.ToString(), *ActionName.ToString());
			
		}
	}
	
	UE_LOGFMT(LogTemp, Log, "[{GameTime}]Action {ActionName} started.", GameTime.GetWorldTimeSeconds(), ActionName.ToString());
	IsRunning = true;
}
