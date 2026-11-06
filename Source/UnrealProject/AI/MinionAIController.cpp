// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAIController.h"

#include "TestProjectGameTypes.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AMinionAIController::AMinionAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
}

void AMinionAIController::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();
	
	SetGenericTeamId(FGenericTeamId(TEAM_ID_ENEMY));
}

// Called when the game starts or when spawned
void AMinionAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
}

