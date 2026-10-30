// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectAnimInstance.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/ActionSystemComponent.h"

void UTestProjectAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (ActionSystemComponent)
	{
		ActionSystemComponent->OnGameplayTagCountChanged.AddDynamic(this, &UTestProjectAnimInstance::OnGameplayTagCountChanged);
	}
}

void UTestProjectAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	ActionSystemComponent = GetOwningActor()->FindComponentByClass<UActionSystemComponent>();
}

void UTestProjectAnimInstance::OnGameplayTagCountChanged(FGameplayTag UpdatedTag, int32 NewCount)
{
	if (UpdatedTag.MatchesTag(SharedGameplayTags::StatusEffect_Sprinting))
	{
		bIsSprinting = NewCount > 0;
	}
	else if (UpdatedTag.MatchesTag(SharedGameplayTags::StatusEffect_Stunned))
	{
		bIsStunned = NewCount > 0;
	}
}
