// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_TargetActor.h"

#include "AIController.h"
#include "TestProjectGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance,	FEnvQueryContextData& ContextData) const
{
	const APawn* QuerierPawn = Cast<APawn>(QueryInstance.Owner.Get());
	if (!QuerierPawn)
	{
		return;
	}

	const AAIController* AIController = Cast<AAIController>(QuerierPawn->GetController());
	if (!AIController)
	{
		return;
	}

	const UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(NAME_TargetActor));
	if (TargetActor)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
	}
}
