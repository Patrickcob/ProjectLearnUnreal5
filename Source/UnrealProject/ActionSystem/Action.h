// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "Action.generated.h"

class UActionSystemComponent;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class UNREALPROJECT_API UAction : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTag ActionName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FGameplayTagContainer BlockedTags;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	float CoolDownTime = 0.0f;
	
	UPROPERTY(Transient)
	float CoolDownUntil = 0.0f;
	
	UPROPERTY(Transient)
	bool IsRunning = false;
	
public:	
	FGameplayTag GetActionName() const { return ActionName; }
	
	float CoolDownTimeRemaining() const { return FMath::Max(0.0f, CoolDownUntil - GetWorld()->GetTimeSeconds()); }
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionSystemComponent* GetOwningComponent() const;
	
	bool CanStart() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction();
};
