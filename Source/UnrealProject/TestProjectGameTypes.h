#pragma once

#include "TestProjectGameTypes.generated.h"

#define NAME_TargetActor "TargetActor"

#define COLLISION_INTERACTION ECC_GameTraceChannel1
#define COLLISION_PROJECTILE ECC_GameTraceChannel2
#define COLLISION_BUFFERS ECC_GameTraceChannel3

class UAIDataAsset;
class UEnvQuery;
class UDataTable;
class AAICharacter;

USTRUCT(BlueprintType)
struct FMonsterSpawnData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAIDataAsset> MonsterDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnWeight = 1.0f;
};

USTRUCT(BlueprintType)
struct FSpawnDirectorData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Spawn System")
	TObjectPtr<UEnvQuery> SpawnLocationQuery;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn System")
	TObjectPtr<UDataTable> SpawnDataTable;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn System")
	FRuntimeFloatCurve CreditGainCurve;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn System")
	float TickInterval = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn System")
	float CoolDown = 6.0f;
	
	float NextTickTime = 0.0f;
	
	float CurrentCredits = 0.0f;
	
	FRandomStream RandomStreamMonsterSelection;
};
