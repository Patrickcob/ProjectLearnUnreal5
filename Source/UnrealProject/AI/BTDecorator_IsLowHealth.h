// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UBTDecorator_IsLowHealth : public UBTDecorator 
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI", meta=(ClampMin="0.0", ClampMax="1.0"))
	float LowHealthFractionThreshold = 0.3f;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
