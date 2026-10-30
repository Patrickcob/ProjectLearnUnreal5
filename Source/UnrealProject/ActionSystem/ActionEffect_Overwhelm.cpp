// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect_Overwhelm.h"

#include "ActionEffect_StunMonster.h"
#include "ActionSystemComponent.h"
#include "SharedGameplayTags.h"

UActionEffect_Overwhelm::UActionEffect_Overwhelm()
{
	// Since the stun for monsters is entirely in C++, we can assign it directly here instead of a Blueprint Class
	EffectOnThreshold = UActionEffect_StunMonster::StaticClass();
	// Note: We do not need any BP derived class for this Overwhelm debuff
}

void UActionEffect_Overwhelm::OnHealthChanged(FGameplayTag AttributeTag, float NewAttributeValue, float OldAttributeValue)
{
	const float Delta = NewAttributeValue - OldAttributeValue;
	if (Delta >= 0.0f)
	{
		// ignore healing
		return;
	}
	
	// Do not count damage until Stunned has expired (avoid stunlocking)
	if (GetOwningComponent()->GetActiveTags().HasTag(SharedGameplayTags::StatusEffect_Stunned))
	{
		return;
	}
	
	const float TimeNow = GetWorld()->TimeSeconds;
	// Did less time pass than the threshold?
	const float DeltaTime = TimeNow - LastDamageTime;
	if (DeltaTime < TimeDeltaThreshold)
	{
		SummedRecentDamage += FMath::Abs(Delta);
	}
	else
	{
		// outside time threshold, reset to only current damage
		SummedRecentDamage = FMath::Abs(Delta);
	}
	
	LastDamageTime = TimeNow;
	
	// Trigger stun?
	if (SummedRecentDamage >= DamageThreshold)
	{
		GetOwningComponent()->GrantAction(EffectOnThreshold);
		
		UE_LOG(LogTemp, Log, TEXT("Stunned applied to %s"), *GetNameSafe(GetOwningComponent()->GetOwner()));
		
		// Reset now that the stun is applied
		SummedRecentDamage = 0.0f;
		LastDamageTime = 0.0f;
	}
}

void UActionEffect_Overwhelm::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	
	HealthDelegateHandle = GetOwningComponent()->GetAttributeChangedDelegate(SharedGameplayTags::Attribute_Health).AddUObject(this, 
		&ThisClass::OnHealthChanged);
}

void UActionEffect_Overwhelm::StopAction_Implementation()
{
	Super::StopAction_Implementation();

	// Clear as soon we stop, otherwise we continue receiving delegates from OnHealthChanged
	GetOwningComponent()->GetAttributeChangedDelegate(SharedGameplayTags::Attribute_Health).Remove(HealthDelegateHandle);
}