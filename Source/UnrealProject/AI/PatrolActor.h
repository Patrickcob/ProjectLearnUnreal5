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
	
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	UPROPERTY(EditAnywhere, Category="Patrol")
	TArray<TObjectPtr<APatrolActor>> NearbyPatrolPoints;
	
	UPROPERTY(EditAnywhere, Category="Patrol")
	float PatrolSearchRadius = 1000.0f;

public:
	TArray<APatrolActor*> GetConnections() const
	{
		return NearbyPatrolPoints;
	}
	
	void RemovePatrolConnection(APatrolActor* OtherPatrolPoint);
	
	void AddPatrolConnection(APatrolActor* OtherPatrolPoint);
	
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;
};
