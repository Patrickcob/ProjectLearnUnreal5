// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttributeSet.generated.h"


class UActionSystemComponent;

USTRUCT()
struct FAttribute
{
	GENERATED_BODY()
	
	FAttribute(){}
	
	FAttribute(float Inbase) : Base(Inbase) {}
	
	UPROPERTY(EditAnywhere, Category="Attributes")
	float Base = 0.0f;
	
	UPROPERTY(Transient)
	float Modifier = 0.0f;
	
	float GetValue() const
	{
		return Base + Modifier;
	}
};

/**
 * 
 */
UCLASS(EditInlineNew)
class UNREALPROJECT_API UAttributeSet : public UObject
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	UActionSystemComponent* GetOwningComponent() const;

public:
	virtual void InitializeAttributes() {}
	virtual void PostProcessAttributeChange() {}
};

UCLASS()
class UHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Health")
	FAttribute Health;
	
	UPROPERTY(EditAnywhere, Category="Health")
	FAttribute MaxHealth;
	
	virtual void PostProcessAttributeChange() override;
	
	UHealthAttributeSet();
};

#pragma region PawnAttributes

UCLASS()
class UPawnAttributeSet : public UHealthAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="MoveSpeed")
	FAttribute MoveSpeed;

	void ApplyMoveSpeed();
	
	virtual void InitializeAttributes() override;
	virtual void PostProcessAttributeChange() override;
	
	UPawnAttributeSet();
};

#pragma endregion

#pragma region PlayerAttributes
UCLASS()
class UPlayerAttributeSet : public UPawnAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Resources")
	FAttribute Rage;
	
	UPROPERTY(EditAnywhere, Category="Resources")
	FAttribute MaxRage;
	
	virtual void PostProcessAttributeChange() override;
	
	UPlayerAttributeSet();
};

#pragma endregion 

#pragma region AIAttributes
UCLASS()
class UAIAttributeSet : public UPawnAttributeSet
{
	GENERATED_BODY()

public:
	UAIAttributeSet();
};

#pragma endregion