#pragma once

#include "TestProjectGameTypes.generated.h"

#define NAME_TargetActor "TargetActor"

#define COLLISION_INTERACTION ECC_GameTraceChannel1
#define COLLISION_PROJECTILE ECC_GameTraceChannel2
#define COLLISION_BUFFERS ECC_GameTraceChannel3

#define TEAM_ID_PLAYER 0
#define TEAM_ID_ENEMY 1

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0.0"))
	float SpawnCost = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0.0"))
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
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn System")
	FString DebugDisplay = TEXT("Spawn Director");
	
	UPROPERTY(EditDefaultsOnly, Category="Spawn System")
	FColor DebugColor = FColor::Green;
	
	float NextTickTime = 0.0f;
	
	float CurrentCredits = 0.0f;
	
	FRandomStream RandomStreamMonsterSelection;
};
