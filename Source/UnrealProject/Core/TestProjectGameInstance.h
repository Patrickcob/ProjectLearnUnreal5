// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TestProjectGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UTestProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<APawn>> AliveMonsters;

	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
};
