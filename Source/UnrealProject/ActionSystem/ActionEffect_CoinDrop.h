// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionEffect.h"
#include "ActionEffect_CoinDrop.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UActionEffect_CoinDrop : public UActionEffect
{
	GENERATED_BODY()
public:
	
	virtual void StartAction_Implementation() override;
	virtual void StopAction_Implementation() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Effect|Coins")
	int32 NumPiles = 3;

	UPROPERTY(EditDefaultsOnly, Category="Effect|Coins")
	int32 CoinsPerPile = 10;

	UPROPERTY(EditDefaultsOnly, Category="Effect|Coins")
	float ScatterRadius = 150.0f;

	FDelegateHandle HealthDelegateHandle;

	void OnHealthChanged(FGameplayTag AttributeTag, float NewValue, float OldValue);
	void SpawnCoins(AActor* InstigatorCharacter);
};
