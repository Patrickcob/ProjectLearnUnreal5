// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinTestActor.h"

#include "NavigationSystem.h"
#include "Buffers/CoinBufferSubsystem.h"


ACoinTestActor::ACoinTestActor()
{
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(DefaultSceneComponent);
}

void ACoinTestActor::SpawnCoins(int32 NumCoins)
{
	TArray<FVector> NewLocations;
	TArray<int32> NewAmounts;
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(this);
	FVector ActorLocation = GetActorLocation();
	
	for (int i = 0; i < NumCoins; ++i)
	{
		FNavLocation NavLocation;
		bool bFound = NavSys->GetRandomPointInNavigableRadius(ActorLocation, 1000.0f, NavLocation);
		if (bFound)
		{
			NewLocations.Add(NavLocation.Location);
			NewAmounts.Add(10); // Each coin buffer will have an amount of 10
		}
	}
	
	UCoinBufferSubsystem* CoinBufferSubsystem = GetWorld()->GetSubsystem<UCoinBufferSubsystem>();
	CoinBufferSubsystem->AddCoinBuffers(NewLocations, NewAmounts);
}
