// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_AlivePlayers.h"
#include "EngineUtils.h"
#include "Core/TestProjectGameplayStatics.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Player/PlayerCharacter.h"

void UEnvQueryContext_AlivePlayers::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	UWorld* World = QueryInstance.World.Get();
	if (!World)
	{
		return;
	}

	TArray<AActor*> AlivePlayers = TArray<AActor*>();
	for (APlayerCharacter* Character : TActorRange<APlayerCharacter>(World))
	{
		if (UTestProjectGameplayStatics::IsAlive(Character))
		{
			AlivePlayers.Add(Character);
		}
	}
	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, AlivePlayers);
}
