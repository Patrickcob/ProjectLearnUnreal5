// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolActor.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"


// Sets default values
APatrolActor::APatrolActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APatrolActor::BeginPlay()
{
	Super::BeginPlay();
	
	//Search nearby PatrolActors and add them to the PatrolPoints array within a search radius
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(this);
	const ANavigationData* NavData = NavSys ? NavSys->GetDefaultNavDataInstance() : nullptr;
	if (!NavData)
	{
		return;
	}
	
	const FVector Start = GetActorLocation();
	for (APatrolActor* Other : TActorRange<APatrolActor>(GetWorld()))
	{
		if (Other == this)
		{
			continue;
		}

		const FVector End = Other->GetActorLocation();

		// The path can't be shorter than the straight line, so skip the pathfinding when that's already too far
		if (FVector::DistSquared(Start, End) > FMath::Square(PatrolSearchRadius))
		{
			continue;
		}

		FPathFindingQuery Query(this, *NavData, Start, End);
		const FPathFindingResult Result = NavSys->FindPathSync(Query);

		if (Result.IsSuccessful() && Result.Path.IsValid() && !Result.Path->IsPartial()
			&& Result.Path->GetLength() <= PatrolSearchRadius)
		{
			PatrolPoints.AddUnique(Other);
		}
	}
}


// Called every frame
void APatrolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Render a debug sphere at the actor's location and a line to each patrol point
	DrawDebugSphere(GetWorld(), GetActorLocation(), 50.0f, 12, FColor::Green, false, -1.0f, 0, 2.0f);
	for (APatrolActor* PatrolPoint : PatrolPoints)
	{
		if (PatrolPoint)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), PatrolPoint->GetActorLocation(), FColor::Blue, false, -1.0f, 0, 2.0f);
		}
	}
}

