// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsLowHealth.h"

#include "AICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/TestProjectGameplayStatics.h"

bool UBTDecorator_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//Check if the AI character's health is below the threshold
	AAICharacter* Pawn = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Pawn)
	{
		bool bIsLowHealth = UTestProjectGameplayStatics::IsLowHealth(Pawn->ActionSystemComponent, LowHealthFractionThreshold);
		
		UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
		check(BBComp);
		
		return bIsLowHealth;
	}
	
	return false;
}
