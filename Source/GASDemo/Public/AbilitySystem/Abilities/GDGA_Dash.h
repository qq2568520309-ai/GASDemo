// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GDGameplayAbility.h"
#include "GDGA_Dash.generated.h"


UCLASS()
class GASDEMO_API UGDGA_Dash : public UGDGameplayAbility
{
	GENERATED_BODY()
public:
	UGDGA_Dash();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> InVulnerableEffect;
	
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	
	
};
