// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIDataAsset.generated.h"

class UAction;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UAIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TSubclassOf<APawn> MonsterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TArray<TSubclassOf<UAction>> MonsterActions;
};
