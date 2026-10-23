// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RangedAttack.generated.h"

class ATestProjectileMagic;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FName MuzzleSocketName;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<ATestProjectileMagic> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float SpreadAngle = 5.0f;

	UBTTask_RangedAttack();
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
