// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet.h"

#include "ActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UActionSystemComponent* UAttributeSet::GetOwningComponent() const
{
	return Cast<UActionSystemComponent>(GetOuter());
}

void UHealthAttributeSet::PostProcessAttributeChange()
{
	Health.Base = FMath::Clamp(Health.GetValue(), 0.0f, MaxHealth.GetValue());
}

UHealthAttributeSet::UHealthAttributeSet()
{
	Health = FAttribute(100.0f);
	MaxHealth = FAttribute(100.0f);
}

void UPawnAttributeSet::ApplyMoveSpeed()
{
	auto Character = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue();
}

void UPawnAttributeSet::InitializeAttributes()
{
	Super::InitializeAttributes();
	ApplyMoveSpeed();
}

void UPawnAttributeSet::PostProcessAttributeChange()
{
	Super::PostProcessAttributeChange();
	
	ApplyMoveSpeed();
}

UPawnAttributeSet::UPawnAttributeSet()
{
	MoveSpeed = FAttribute(600.0f);
}

UPlayerAttributeSet::UPlayerAttributeSet()
{
	Rage = FAttribute(0.0f);
	MaxRage = FAttribute(100.0f);
}

UAIAttributeSet::UAIAttributeSet()
{
	MoveSpeed = FAttribute(450.0f);
}
