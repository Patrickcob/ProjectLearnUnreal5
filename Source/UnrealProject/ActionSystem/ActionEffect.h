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
	
	UPROPERTY(EditDefaultsOnly, Category="Effect") 
	float Period = 0.0f;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Effect") 
	int32 StackCount = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	bool bResetDurationOnStackIncrease = true;
	
	FTimerHandle DurationTimerHandle;
	FTimerHandle PeriodTimerHandle;
	
	UFUNCTION(BlueprintNativeEvent, Category="Effect")
	void ExecutePeriodicEffect();
	
	void ResetDuration();


public:
	void IncrementStackSize();
	
	virtual void StartAction_Implementation() override;
	
	virtual void StopAction_Implementation() override;
};
