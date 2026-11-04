// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectGameInstance.h"

void UTestProjectGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
	
	//Changing worlds, clear the list of alive monsters to avoid dangling references
	ensure(AliveMonsters.Num() == 0);
}
