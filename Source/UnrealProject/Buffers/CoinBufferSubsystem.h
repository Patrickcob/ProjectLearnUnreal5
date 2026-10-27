// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CoinBufferSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UCoinBufferSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	
	void AddCoinBuffers(TArray<FVector> NewLocations, TArray<int32> NewAmounts);

	void RemoveCoinBuffer(int Element);
	 virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(CoinBufferSubsystem, STATGROUP_Tickables);
	}
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

protected:
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> WorldISMComponent;
	
	TArray<FVector> Locations;
	TArray<int32> Amounts;
	TArray<FPrimitiveInstanceId> InstanceIDs;
};
