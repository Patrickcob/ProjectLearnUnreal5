// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"

#include "ActionSystem/ActionSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AMinionAIController::AMinionAIController()
{
}

// Called when the game starts or when spawned
void AMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn == nullptr)
	{
		return;
	}
	
	GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"), PlayerPawn);
}

