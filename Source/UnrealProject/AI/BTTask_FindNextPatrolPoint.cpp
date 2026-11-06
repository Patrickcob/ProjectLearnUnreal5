#include "BTTask_FindNextPatrolPoint.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "PatrolActor.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTask_FindNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	APatrolActor* CurrentPatrolPoint = Cast<APatrolActor>(BBComp->GetValueAsObject(CurrentPatrolPointKey.SelectedKeyName));
	if (!IsValid(CurrentPatrolPoint))
	{
		CurrentPatrolPoint = FindNearestPatrolActor(OwnerComp);
		
		// If we still didn't find anything, we can't do much more...
		if (CurrentPatrolPoint == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No PatrolPoint available for %s"), *GetNameSafe(OwnerComp.GetOwner()));
			return EBTNodeResult::Failed;
		}
	}

	TArray<APatrolActor*> Connections = CurrentPatrolPoint->GetConnections();
	if (Connections.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PatrolPoint %s has no connections available for %s"), 
			*GetNameSafe(CurrentPatrolPoint), *GetNameSafe(OwnerComp.GetOwner()));
		return EBTNodeResult::Failed;
	}

	APatrolActor* OldPatrolPoint = Cast<APatrolActor>(BBComp->GetValueAsObject(PrevPatrolPointKey.SelectedKeyName));
	// Store the "current" as the new previous patrol
	BBComp->SetValueAsObject(PrevPatrolPointKey.SelectedKeyName, CurrentPatrolPoint);
	
	if (Connections.Num() > 1)
	{
		// Remove previous point to avoid re-picking
		Connections.RemoveSingle(OldPatrolPoint);
	}
	
	// Randomly pick new patrol point
	const int32 RandomIndex = FMath::RandRange(0, Connections.Num()-1);
	APatrolActor* NewPoint = Connections[RandomIndex];
	BBComp->SetValueAsObject(CurrentPatrolPointKey.SelectedKeyName, NewPoint);
	
	return EBTNodeResult::Succeeded;
}

APatrolActor* UBTTask_FindNextPatrolPoint::FindNearestPatrolActor(UBehaviorTreeComponent& OwnerComp)
{
	APatrolActor* NearestPatrolActor = nullptr;
	float NearestDistSqrd = MAX_FLT;
	
	// Assume we have a Pawn at all which is true for our current implementation, otherwise 
	// it may need to be called later in the init sequence
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	check(AIPawn);
	FVector PawnLocation = AIPawn->GetActorLocation();
		
	// Find nearest, iterating ALL patrol points.
	for (APatrolActor* PatrolActor : TActorRange<APatrolActor>(GetWorld()))
	{
		float DistTo = FVector::DistSquared(PawnLocation, PatrolActor->GetActorLocation());
		if (DistTo < NearestDistSqrd)
		{
			NearestDistSqrd = DistTo;
			NearestPatrolActor = PatrolActor;
		}
	}

	return NearestPatrolActor;
}