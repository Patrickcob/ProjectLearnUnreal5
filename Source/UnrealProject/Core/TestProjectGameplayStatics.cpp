// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectGameplayStatics.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/ActionSystemComponent.h"

bool UTestProjectGameplayStatics::IsFullHealth(UActionSystemComponent* ActionSystemComponent)
{
	if (!ActionSystemComponent)
	{
		return false;
	}

	auto CurrentHealth = ActionSystemComponent->GetAttribute(SharedGameplayTags::Attribute_Health);
	auto MaxHealth = ActionSystemComponent->GetAttribute(SharedGameplayTags::Attribute_MaxHealth);

	return FMath::IsNearlyEqual(CurrentHealth->GetValue(), MaxHealth->GetValue());
}

bool UTestProjectGameplayStatics::IsLowHealth(UActionSystemComponent* ActionSystemComponent, float LowHealthThreshold)
{
	if (!ActionSystemComponent)
	{
		return false;
	}

	auto CurrentHealth = ActionSystemComponent->GetAttribute(SharedGameplayTags::Attribute_Health);
	auto MaxHealth = ActionSystemComponent->GetAttribute(SharedGameplayTags::Attribute_MaxHealth);

	return CurrentHealth->GetValue() < (MaxHealth->GetValue() * LowHealthThreshold);
}
