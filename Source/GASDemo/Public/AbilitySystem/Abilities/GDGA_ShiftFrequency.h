// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GDGameplayAbility.h"
#include "GDGA_ShiftFrequency.generated.h"


UCLASS()
class GASDEMO_API UGDGA_ShiftFrequency : public UGDGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGDGA_ShiftFrequency();
	
protected:
	virtual  void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
