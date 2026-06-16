// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GDAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GASDEMO_API UGDAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Health)
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Energy)
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxEnergy)
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Shield)
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxShield)
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData Heat;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Heat)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Sync;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Sync)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxSync;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxSync)
	
	UPROPERTY(BlueprintReadOnly,Category = "Attributes")
	FGameplayAttributeData MaxHeat;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxHeat)
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MoveSpeed)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Damage)
	
};
