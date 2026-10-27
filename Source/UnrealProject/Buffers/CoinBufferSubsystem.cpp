// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinBufferSubsystem.h"

#include "EngineUtils.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Player/PlayerCharacter.h"

void UCoinBufferSubsystem::AddCoinBuffers(TArray<FVector> NewLocations, TArray<int32> NewAmounts)
{
	Locations.Append(NewLocations);
	Amounts.Append(NewAmounts);
	
	TArray<FTransform> NewTransforms;
	for (int i = 0; i < NewLocations.Num(); ++i)
	{
		NewTransforms.Add(FTransform(NewLocations[i]));
	}
	
	TArray<FPrimitiveInstanceId> NewInstanceIds = WorldISMComponent->AddInstancesById(NewTransforms, true, false);
	InstanceIDs.Append(NewInstanceIds);
}

void UCoinBufferSubsystem::RemoveCoinBuffer(int Element)
{
	if (Locations.IsValidIndex(Element) && Amounts.IsValidIndex(Element))
	{
		Locations.RemoveAt(Element);
		Amounts.RemoveAt(Element);
		WorldISMComponent->RemoveInstanceById(InstanceIDs[Element]);
		InstanceIDs.RemoveAt(Element);
	}
}

void UCoinBufferSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UWorld* World = GetWorld();
	
	FVector PlayerLocation = FVector::ZeroVector;
	for (APlayerCharacter* PlayerCharacter: TActorRange<APlayerCharacter>(World))
	{
		PlayerLocation = PlayerCharacter->GetActorLocation();
		break;
	}
	
	const float PickupRadius = 100.0f;
	
	TArray<int32> ProcessList;

	for (int i = 0; i < Locations.Num(); ++i)
	{
		float Distance = FVector::Dist(PlayerLocation, Locations[i]);
		if (Distance <= PickupRadius)
		{
			// Handle coin pickup logic here
			ProcessList.Add(i);
		}
	}

	int32 TotalCoins = 0;
	for (int i = ProcessList.Num() - 1; i >= 0; --i)
	{
		TotalCoins += Amounts[ProcessList[i]];
		RemoveCoinBuffer(ProcessList[i]);
	}
	
	for (int32 i = Locations.Num() - 1; i >= 0; --i)
	{
		DrawDebugPoint(World, Locations[i], 8.0f, FColor::Yellow);
	}
}

void UCoinBufferSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	UWorld* World = GetWorld();
	
	//TODO: Async load instead
	FSoftObjectPath MeshPath(TEXT("/Game/ExampleContent/Meshes/SM_Pickup_Coin.SM_Pickup_Coin"));
	UStaticMesh* LoadedMesh = Cast<UStaticMesh>(MeshPath.TryLoad());
	
	WorldISMComponent = NewObject<UInstancedStaticMeshComponent>(World, NAME_None, RF_Transient);
	WorldISMComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WorldISMComponent->SetStaticMesh(LoadedMesh);
	WorldISMComponent->RegisterComponentWithWorld(World);
}
