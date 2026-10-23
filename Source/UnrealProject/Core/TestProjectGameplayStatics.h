// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestProjectGameplayStatics.generated.h"

class UActionSystemComponent;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UTestProjectGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	static bool IsFullHealth(UActionSystemComponent * ActionSystemComponent);
	static bool IsLowHealth(UActionSystemComponent * ActionSystemComponent, float LowHealthThreshold);
};
