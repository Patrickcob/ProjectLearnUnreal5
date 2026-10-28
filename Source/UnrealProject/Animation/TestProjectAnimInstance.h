// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TestProjectAnimInstance.generated.h"

class UActionSystemComponent;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UTestProjectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Status Effects")
	bool bIsSprinting;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category="Status Effects")
	bool bIsStunned;
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<UActionSystemComponent> ActionSystemComponent;
};
