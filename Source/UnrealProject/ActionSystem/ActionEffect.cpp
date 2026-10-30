// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect.h"

#include "ActionSystemComponent.h"

void UActionEffect::ExecutePeriodicEffect_Implementation()
{
}

void UActionEffect::ResetDuration()
{
	GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, this, &ThisClass::StopAction, Duration);
}

void UActionEffect::IncrementStackSize()
{
	StackCount++;

	if (bResetDurationOnStackIncrease)
	{
		ResetDuration();
	}
}

void UActionEffect::StartAction_Implementation()
{
	Super::StartAction_Implementation();

	if (Duration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, this, &ThisClass::StopAction, Duration);
	}

	if (Period > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(PeriodTimerHandle, this, &ThisClass::ExecutePeriodicEffect, Period, true);
	}
}

void UActionEffect::StopAction_Implementation()
{
	if (Period > 0.0f && GetWorld()->GetTimerManager().GetTimerRemaining(PeriodTimerHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect();
	}
	
	Super::StopAction_Implementation();
	
	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodTimerHandle);
	
	GetOwningComponent()->RemoveAction(ActionName);
}
