// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestProjectPlayerController.generated.h"

class UInputAction;
class UTestProjectInteractionComponent;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API ATestProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UTestProjectInteractionComponent> InteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Input_Interact;
	
	void Interact();

	virtual void SetupInputComponent() override;
	
public:
	ATestProjectPlayerController();
};
