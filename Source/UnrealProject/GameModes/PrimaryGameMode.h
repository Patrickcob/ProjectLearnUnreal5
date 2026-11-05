// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/TestProjectGameMode.h"
#include "PrimaryGameMode.generated.h"

struct FSpawnDirectorData;
struct FEnvQueryResult;
struct FMonsterSpawnData;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API APrimaryGameMode : public ATestProjectGameMode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TArray<FSpawnDirectorData> SpawnDirectors;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 MaxBotLimit = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	int32 GlobalStartupSeed = 0;

	bool TrySpawnMonster(FSpawnDirectorData& DirectorSpawnData);
public:
	virtual void StartPlay() override;

	void OnSpawnLocationQueryFinished(TSharedPtr<FEnvQueryResult> EnvQueryResult, FMonsterSpawnData* SpawnData);
	void OnMonsterClassLoaded(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject, FVector SpawnLocation, FMonsterSpawnData* SelectedMonster);
	virtual void Tick(float DeltaSeconds) override;

	APrimaryGameMode();
};
