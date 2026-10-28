// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "ActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UActionEffect : public UAction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float Duration = 0.0f;
	
	FTimerHandle DurationTimerHandle;
	
public:
	virtual void StartAction_Implementation() override;
	
	virtual void StopAction_Implementation() override;
};
