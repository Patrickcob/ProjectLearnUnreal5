// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TestProjectDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig)
class UNREALPROJECT_API UTestProjectDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Config, EditDefaultsOnly, Category="Coin Buffer")
	TSoftObjectPtr<UStaticMesh> CoinBufferMesh;
	
	UPROPERTY(Config, EditDefaultsOnly, Category="Coin Buffer")
	TSoftObjectPtr<USoundBase> CoinBufferPickupSound;
	
	UPROPERTY(Config, EditDefaultsOnly, Category="Coin Buffer")
	FName CoinBufferPickupSoundParameter;

	virtual FName GetCategoryName() const override
	{
		return FApp::GetProjectName();
	}
};
