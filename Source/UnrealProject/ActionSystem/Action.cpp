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
	return !GetOwningComponent()->ActiveTags.HasAny(BlockedTags) && !IsRunning && FMath::IsNearlyZero(CoolDownTimeRemaining());
}

void UAction::StopAction_Implementation()
{
	auto GameTime = GetWorld()->GetTime();
	
	UE_LOGFMT(LogTemp, Log, "[{GameTime}]Action {ActionName} stopped.", GameTime.GetWorldTimeSeconds(), ActionName.ToString());
	
	CoolDownUntil = GetWorld()->GetTimeSeconds() + CoolDownTime;
	IsRunning = false;
	
	GetOwningComponent()->ActiveTags.RemoveTags(GrantsTags);
}

void UAction::StartAction_Implementation()
{
	auto GameTime = GetWorld()->GetTime();
	
	UE_LOGFMT(LogTemp, Log, "[{GameTime}]Action {ActionName} started.", GameTime.GetWorldTimeSeconds(), ActionName.ToString());
	IsRunning = true;
	
	GetOwningComponent()->ActiveTags.AppendTags(GrantsTags);
}
