// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectAnimInstance.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/ActionSystemComponent.h"

void UTestProjectAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	ActionSystemComponent = GetOwningActor()->FindComponentByClass<UActionSystemComponent>();
}

void UTestProjectAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	bIsSprinting = ActionSystemComponent && ActionSystemComponent->ActiveTags.HasTag(SharedGameplayTags::StatusEffect_Sprinting);
	bIsStunned = ActionSystemComponent && ActionSystemComponent->ActiveTags.HasTag(SharedGameplayTags::StatusEffect_Stunned);
}
