// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryGameMode.h"

#include "TestProjectGameTypes.h"
#include "UnrealProject.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "AI/AICharacter.h"
#include "AI/AIDataAsset.h"
#include "Core/TestProjectGameInstance.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"

APrimaryGameMode::APrimaryGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;
}

void APrimaryGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float TotalElapsedTime = GetWorld()->TimeSeconds;
	
	UTestProjectGameInstance* GI = GetGameInstance<UTestProjectGameInstance>();
	if (GI->AliveMonsters.Num() > MaxBotLimit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max bot limit reached: %d"), GI->AliveMonsters.Num());
		return;
	}
	
	int32 KeyID = ONSCREENDEBUGKEY_SPAWN_DIRECTOR;
	for (FSpawnDirectorData& DirectorData : SpawnDirectors)
	{
		if (DirectorData.SpawnDataTable == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Data Table is not set"));
			return;
		}

		float CreditsPerSecond = DirectorData.CreditGainCurve.GetRichCurve()->Eval(TotalElapsedTime) * DeltaSeconds;
		DirectorData.CurrentCredits += CreditsPerSecond;
		
		GEngine->AddOnScreenDebugMessage(KeyID, PrimaryActorTick.TickInterval, FColor::Green, FString::Printf(TEXT("Current Credits: %f\nNext Tick Time: %f"), DirectorData.CurrentCredits, DirectorData.NextTickTime));

		if (DirectorData.NextTickTime > TotalElapsedTime)
		{
			continue;			
		}
		
		bool bSuccess = TrySpawnMonster(DirectorData);
		DirectorData.NextTickTime = TotalElapsedTime + (bSuccess ? DirectorData.TickInterval : DirectorData.CoolDown);
		
		
		KeyID++;
	}
}

bool APrimaryGameMode::TrySpawnMonster(FSpawnDirectorData& DirectorData)
{
	TArray<FMonsterSpawnData*> SpawnDataRows;
	DirectorData.SpawnDataTable->GetAllRows("SelectMonster", SpawnDataRows);
	
	//int32 RandomIndex = DirectorData.RandomStreamMonsterSelection.RandRange(0, SpawnDataRows.Num() - 1);
	//FMonsterSpawnData* RandomSpawnData = SpawnDataRows[RandomIndex];

	float TotalWeight = 0.0f;
	for (FMonsterSpawnData* SpawnDataRow : SpawnDataRows)
	{
		TotalWeight += SpawnDataRow->SpawnWeight;
	}
	
	float RandomWeight = DirectorData.RandomStreamMonsterSelection.FRandRange(0.0f, TotalWeight);

	TotalWeight = 0.0f;
	FMonsterSpawnData* RandomSpawnData = nullptr;
	for (FMonsterSpawnData* SpawnDataRow : SpawnDataRows)
	{
		TotalWeight += SpawnDataRow->SpawnWeight;
		if (RandomWeight <= TotalWeight)
		{
			RandomSpawnData = SpawnDataRow;
			break;
		}
	}

	if (RandomSpawnData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to select a monster to spawn"));
		return false;
	}
	
	if (DirectorData.CurrentCredits < RandomSpawnData->SpawnCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough credits to spawn monster %s"), *RandomSpawnData->MonsterDataAsset.GetAssetName());
		return false;
	}
	
	DirectorData.CurrentCredits -= RandomSpawnData->SpawnCost;
	
	FQueryFinishedSignature SpawnLocationQueryFinished = FQueryFinishedSignature::CreateUObject(this, &APrimaryGameMode::OnSpawnLocationQueryFinished, RandomSpawnData);
	
	FEnvQueryRequest Request(DirectorData.SpawnLocationQuery, this);
	int32 Result = Request.Execute(EEnvQueryRunMode::SingleResult, SpawnLocationQueryFinished);
	
	return Result != INDEX_NONE;
}

void APrimaryGameMode::StartPlay()
{
	Super::StartPlay();
	
	FRandomStream GlobalStream = FRandomStream(GlobalStartupSeed);

	for (FSpawnDirectorData& Director : SpawnDirectors)
	{
		Director.RandomStreamMonsterSelection = FRandomStream(GlobalStream.FRandRange(0, MAX_int32 - 1));
	}
}

void APrimaryGameMode::OnSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult, FMonsterSpawnData* SpawnData)
{
	if (EnvQueryResult->IsSuccessful())
	{
		FVector SpawnLocation = EnvQueryResult->GetItemAsLocation(0);
		UE_LOG(LogTemp, Log, TEXT("Spawn Location: %s"), *SpawnLocation.ToString());
		
		SpawnData->MonsterDataAsset.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &APrimaryGameMode::OnMonsterClassLoaded, SpawnLocation, SpawnData));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Location Query Failed"));
	}
}

void APrimaryGameMode::OnMonsterClassLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject, FVector SpawnLocation, FMonsterSpawnData* SelectedMonster)
{
	
	if (LoadedObject)
	{
		UAIDataAsset* MonsterDataAsset = SelectedMonster->MonsterDataAsset.Get();
		
		FActorSpawnParameters SpawnParams;
		AAICharacter* SpawnedMonster = GetWorld()->SpawnActorDeferred<AAICharacter>(MonsterDataAsset->MonsterClass, FTransform::Identity);
		
		UE_VLOG_SPHERE(this, LogGameMode, Log, SpawnLocation, 50.0f, FColor::Green, TEXT("Spawned Monster: %s with cost %s"), *MonsterDataAsset->MonsterClass->GetName(), *FString::SanitizeFloat(SelectedMonster->SpawnCost));
		
		UE_LOG(LogTemp, Log, TEXT("Spawned Monster: %s at Location: %s"), *MonsterDataAsset->MonsterClass->GetName(), *SpawnLocation.ToString());
		
		SpawnedMonster->SetMonsterAsset(MonsterDataAsset);
		
		UGameplayStatics::FinishSpawningActor(SpawnedMonster, FTransform(SpawnLocation));
		
		UActionSystemComponent* ActionSystemComponent = SpawnedMonster->ActionSystemComponent;
		for (TSubclassOf<UAction> Action : MonsterDataAsset->MonsterActions)
		{
			ActionSystemComponent->GrantAction(Action);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load Monster Class"));
	}
}

