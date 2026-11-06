// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolActor.h"

#include "EngineUtils.h"
#include "NavigationSystem.h"


// Sets default values
APatrolActor::APatrolActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
#if WITH_EDITORONLY_DATA
	// Just like if we add a default scene root in Blueprint, we want to show an Icon to keep the Actor selectable
	DefaultSceneRoot->bVisualizeComponent = true;
#endif
	RootComponent = DefaultSceneRoot;
}

void APatrolActor::RemovePatrolConnection(APatrolActor* OtherPatrolPoint)
{
	int32 RemoveCount = NearbyPatrolPoints.RemoveSingle(OtherPatrolPoint);
	if (RemoveCount > 0)
	{
		MarkPackageDirty();
	}
}

void APatrolActor::AddPatrolConnection(APatrolActor* OtherPatrolPoint)
{
	// Avoid duplicates and prevent marking assets as dirty when unchanged
	if (!NearbyPatrolPoints.Contains(OtherPatrolPoint))
	{
		NearbyPatrolPoints.Add(OtherPatrolPoint);
		MarkPackageDirty();
	}
}

void APatrolActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	UWorld* World = GetWorld();
	float DebugDuration = 2.0f;
	const FVector StartLocation = GetActorLocation();
	
	// by the end IF they are no longer in the nearby patrol list, they should also update to remove "this" from their list.
	TArray<TWeakObjectPtr<APatrolActor>> PrevPatrolPoints(NearbyPatrolPoints);
	TArray<APatrolActor*> ValidPatrolPoints;

	for (APatrolActor* OtherPatrolPoint : TActorRange<APatrolActor>(World))
	{
		if (OtherPatrolPoint == this)
		{
			// skip self
			continue;
		}
		
		FVector OtherLocation = OtherPatrolPoint->GetActorLocation();
		// Don't even attempt nav path if we are further away in straight line
		if (FVector::Distance(StartLocation, OtherLocation) > PatrolSearchRadius)
		{
			continue;
		}

		double PathLength = 0;
		ENavigationQueryResult::Type Result = UNavigationSystemV1::GetPathLength(this, StartLocation, OtherLocation, PathLength);
		if (Result == ENavigationQueryResult::Error)
		{
			// Quit trying, nav data isn't available yet
			UE_LOG(LogTemp, Warning, TEXT("Unable to make patrol points, nav data not ready."));
			return;
		}
		
		if (Result == ENavigationQueryResult::Success && PathLength <= PatrolSearchRadius)
		{
			ValidPatrolPoints.Add(OtherPatrolPoint);
			NearbyPatrolPoints.Add(OtherPatrolPoint);
			// Add for the other patrol point too to keep it in sync
			OtherPatrolPoint->AddPatrolConnection(this);
			
			DrawDebugBox(World, OtherLocation, FVector(20.f), FColor::Green, false, DebugDuration);
			DrawDebugDirectionalArrow(World, StartLocation, OtherLocation, 64.0f, FColor::Green, false, DebugDuration);
		}
	}
	
	// MoveTemp can be used to move the data, you can't use the data in ValidPatrolPoints itself after this call anymore
	NearbyPatrolPoints = MoveTemp(ValidPatrolPoints);
	
	// remove any no longer valid
	for (TWeakObjectPtr<APatrolActor> WeakOldPoint : PrevPatrolPoints)
	{
		APatrolActor* OldPoint = WeakOldPoint.Get();
		if (!IsValid(OldPoint) || NearbyPatrolPoints.Contains(OldPoint))
		{
			continue;
		}
		
		if (!NearbyPatrolPoints.Contains(OldPoint))
		{
			// No longer a connection, push update on the other patrol point to keep it in sync
			OldPoint->RemovePatrolConnection(this);
			
			FVector OtherLocation = OldPoint->GetActorLocation();
			DrawDebugDirectionalArrow(World, StartLocation, OtherLocation, 64.0f, FColor::Orange, false, DebugDuration);
			DrawDebugBox(World, OtherLocation, FVector(20.f), FColor::Orange, false, DebugDuration);
		}
	}
	
}

void APatrolActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Draw the patrol search radius
	DrawDebugSphere(GetWorld(), GetActorLocation(), PatrolSearchRadius, 32.0f, FColor::Green, false, -1.0f, 0, 2.0f);
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(20.f), FColor::Orange, false, -1.0f, 0, 2.0f);

	//debug draw the patrol connections
	for (TObjectPtr<APatrolActor> OtherPatrolPoint : NearbyPatrolPoints)
	{
		if (IsValid(OtherPatrolPoint))
		{
			DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), OtherPatrolPoint->GetActorLocation(), 64.0f, FColor::Blue, false, -1.0f, 0, 2.0f);
		}
	}
}
