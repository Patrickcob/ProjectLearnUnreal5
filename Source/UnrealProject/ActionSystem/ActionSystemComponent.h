// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ActionSystemComponent.generated.h"

class UAttributeSet;
class UAction;

UENUM(BlueprintType)
enum EAttributeModifyType
{
	Base,
	Modifier,
	OverrideBase,
	Invalid
};

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewAttributeValue*/, float /*OldAttributeValue*/);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAttributeDynamicChanged, FGameplayTag, AttributeTag, float, NewAttributeValue, float, OldAttributeValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALPROJECT_API UActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void ApplyAttributeChange(FGameplayTag InAttributeTag, float Delta, EAttributeModifyType ModifyType = Base);
	
	void StartAction(FGameplayTag InActionName);
	
	void StopAction(FGameplayTag InActionName);
	
	// Sets default values for this component's properties
	UActionSystemComponent();
	
	UPROPERTY(Transient)
	FGameplayTagContainer ActiveTags;
	
	FAttribute* GetAttribute(FGameplayTag InAttributeTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Action System")
	float GetAttributeValue(FGameplayTag InAttributeTag) const;
	
	FOnAttributeChanged& GetAttributeChangedDelegate(FGameplayTag InAttributeTag) { return AttributeChangedDelegates.FindOrAdd(InAttributeTag); }
	
	UFUNCTION(BlueprintCallable, Category = "Action System")
	void AddDynamicAttributeChangedDelegate(const FGameplayTag InAttributeTag, const FOnAttributeDynamicChanged& InDelegate);
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> Attributes;
	
	TMap<FGameplayTag, FAttribute*> AttributesByTag;
	
	UPROPERTY(EditAnywhere, Category = "Attributes", NoClear)
	TSubclassOf<UAttributeSet> AttributeSetClass;
	
	TMap<FGameplayTag, FOnAttributeChanged> AttributeChangedDelegates;
	
	TMap<FGameplayTag, TArray<FOnAttributeDynamicChanged>> AttributeDynamicChangedDelegates;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TMap<FGameplayTag, TObjectPtr<UAction>> Actions;
		
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UAction>> DefaultActions;
};
