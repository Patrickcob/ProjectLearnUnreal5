// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect.h"

#include "ActionSystemComponent.h"

void UActionEffect::StartAction_Implementation()
{
	Super::StartAction_Implementation();

	if (Duration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, this, &ThisClass::StopAction, Duration);
	}
}

void UActionEffect::StopAction_Implementation()
{
	Super::StopAction_Implementation();
	
	GetWorld()->GetTimerManager().ClearTimer(DurationTimerHandle);
	
	GetOwningComponent()->RemoveAction(ActionName);
}
