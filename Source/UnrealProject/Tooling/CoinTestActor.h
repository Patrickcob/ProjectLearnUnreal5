// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoinTestActor.generated.h"

UCLASS()
class UNREALPROJECT_API ACoinTestActor : public AActor
{
	GENERATED_BODY()
public:
	ACoinTestActor();
	
	UFUNCTION(BlueprintCallable)
	void SpawnCoins(int32 NumCoins);
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USceneComponent> DefaultSceneComponent;
};
