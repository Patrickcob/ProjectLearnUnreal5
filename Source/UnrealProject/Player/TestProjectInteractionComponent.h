// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestProjectInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALPROJECT_API UTestProjectInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionRadius = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float DistanceToWeightScale = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float DirectionWeightScale = 1.0f;
	
	UPROPERTY()
	TObjectPtr<AActor> SelectedActor;

public:
	// Sets default values for this component's properties
	UTestProjectInteractionComponent();
	
	void Interact();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
