// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect_CoinDrop.h"

#include "ActionSystemComponent.h"
#include "NavigationSystem.h"
#include "SharedGameplayTags.h"
#include "Buffers/CoinBufferSubsystem.h"

void UActionEffect_CoinDrop::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	HealthDelegateHandle = GetOwningComponent()->GetAttributeChangedDelegate(SharedGameplayTags::Attribute_Health)
		.AddUObject(this, &ThisClass::OnHealthChanged);
}

void UActionEffect_CoinDrop::StopAction_Implementation()
{
	Super::StopAction_Implementation();
	GetOwningComponent()->GetAttributeChangedDelegate(SharedGameplayTags::Attribute_Health).Remove(HealthDelegateHandle);
}

void UActionEffect_CoinDrop::OnHealthChanged(FGameplayTag AttributeTag, float NewValue, float OldValue)
{
	if (!FMath::IsNearlyZero(NewValue) || FMath::IsNearlyZero(OldValue))
	{
		return;
	}

	SpawnCoins(GetOwningComponent()->GetOwner());
	StopAction();
}


void UActionEffect_CoinDrop::SpawnCoins(AActor* InstigatorCharacter)
{
	TArray<FVector> NewLocations;
	TArray<int32> NewAmounts;
	
	FVector ActorLocation = InstigatorCharacter->GetActorLocation();
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(this);
	
	int32 RandomValueNumCoins = FMath::RandRange(1, 10);
	for (int i = 0; i < RandomValueNumCoins; ++i)
	{
		FNavLocation NavLocation;
		bool bFound = NavSys->GetRandomPointInNavigableRadius(ActorLocation, ScatterRadius, NavLocation);
		if (bFound)
		{
			int32 RandomValuePerCoin = FMath::RandRange(1, 10);

			NewLocations.Add(NavLocation.Location);
			NewAmounts.Add(RandomValuePerCoin); // Each coin buffer will have an amount value of 10
		}
	}
		
	UCoinBufferSubsystem* CoinBufferSubsystem = GetWorld()->GetSubsystem<UCoinBufferSubsystem>();
	CoinBufferSubsystem->AddCoinBuffers(NewLocations, NewAmounts);
}
