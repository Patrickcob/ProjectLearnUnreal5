// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolActor.generated.h"

UCLASS()
class UNREALPROJECT_API APatrolActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrolActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Patrol")
	TArray<TObjectPtr<APatrolActor>> PatrolPoints;
	
	UPROPERTY(EditAnywhere, Category="Patrol")
	float PatrolSearchRadius = 1000.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
