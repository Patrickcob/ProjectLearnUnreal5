#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNextPatrolPoint.generated.h"

class APatrolActor;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UBTTask_FindNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
		
	UPROPERTY(EditAnywhere, Category="Patrolling")
	FBlackboardKeySelector CurrentPatrolPointKey;
	
	/* Previous patrol point, to skip during selection of the next point */
	UPROPERTY(EditAnywhere, Category="Patrolling")
	FBlackboardKeySelector PrevPatrolPointKey;
	
	APatrolActor* FindNearestPatrolActor(UBehaviorTreeComponent& OwnerComp);

public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};