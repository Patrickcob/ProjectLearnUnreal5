// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MinionAIController.generated.h"

class UActionSystemComponent;
class UBehaviorTree;

UCLASS()
class UNREALPROJECT_API AMinionAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMinionAIController();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	virtual void BeginPlay() override;
};
