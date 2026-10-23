// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectPlayerController.h"

#include "EnhancedInputComponent.h"
#include "TestProjectInteractionComponent.h"

ATestProjectPlayerController::ATestProjectPlayerController()
{
	InteractionComponent = CreateDefaultSubobject<UTestProjectInteractionComponent>(TEXT("InteractionComponent"));
}

void ATestProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &ATestProjectPlayerController::Interact);
}

void ATestProjectPlayerController::Interact()
{
	if (InteractionComponent)
	{
		InteractionComponent->Interact();
	}
}