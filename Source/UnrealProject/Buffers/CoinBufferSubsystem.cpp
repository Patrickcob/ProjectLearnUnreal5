// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinBufferSubsystem.h"

#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/TestProjectDeveloperSettings.h"
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

void UCoinBufferSubsystem::PlayPickupSound()
{
	if (!PickupAudioComponent->IsPlaying())
	{
		PickupAudioComponent->Play();
	}
		
	PickupAudioComponent->SetTriggerParameter(PickupSoundParameter);
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

	if (TotalCoins > 0)
	{
		PlayPickupSound();
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
	
	const UTestProjectDeveloperSettings* Settings = GetDefault<UTestProjectDeveloperSettings>();
	
	PickupSoundParameter = Settings->CoinBufferPickupSoundParameter;
	
	WorldISMComponent = NewObject<UInstancedStaticMeshComponent>(World, NAME_None, RF_Transient);
	WorldISMComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WorldISMComponent->RegisterComponentWithWorld(World);
	
	auto coinBufferHandle = Settings->CoinBufferMesh.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnCoinBufferMeshLoaded));
	
	PickupAudioComponent = NewObject<UAudioComponent>(World, NAME_None, RF_Transient);
	PickupAudioComponent->bAutoActivate = false;
	PickupAudioComponent->RegisterComponentWithWorld(World);
	
	auto pickupSoundHandle = Settings->CoinBufferPickupSound.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnPickupSoundLoaded));

}

void UCoinBufferSubsystem::OnCoinBufferMeshLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject)
{
	WorldISMComponent->SetStaticMesh(Cast<UStaticMesh>(LoadedObject));
}

void UCoinBufferSubsystem::OnPickupSoundLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject)
{
	PickupAudioComponent->SetSound(Cast<USoundBase>(LoadedObject));
}
