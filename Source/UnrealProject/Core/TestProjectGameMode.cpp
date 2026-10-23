// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectGameMode.h"

#include "Player/TestProjectPlayerController.h"

ATestProjectGameMode::ATestProjectGameMode()
{
	PlayerControllerClass = ATestProjectPlayerController::StaticClass();
}
