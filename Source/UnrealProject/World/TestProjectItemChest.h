// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/TestProjectInteraction.h"
#include "TestProjectItemChest.generated.h"

UCLASS()
class UNREALPROJECT_API ATestProjectItemChest : public AActor, public ITestProjectInteraction
{
	GENERATED_BODY()

protected:
	float CurrentLidPitch = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float LidInterpolationSpeed = 60.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float TargetLidPitch = 120.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ChestStaticMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> LidStaticMesh;
	
	void OpenChest(float DeltaTime);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Components")
	void ChestAnimationComplete();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	ATestProjectItemChest();

	virtual void Interact_Implementation() override;
};
