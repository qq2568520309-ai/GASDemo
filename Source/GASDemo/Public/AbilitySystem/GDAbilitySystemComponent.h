// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GDAbilitySystemComponent.generated.h"

UCLASS()
class GASDEMO_API UGDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UGDAbilitySystemComponent();
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	
	
	void SetCurrentFrequencyTag(const FGameplayTag FrequencyTag);
	
	bool GetCooldownRemainingForTag(const FGameplayTag& CooldownTag,float& OutRemainingTime,float& OutDuration) const;
	
	
	FGameplayTag GetCurrentFrequencyTag() const;
	
private:
	
	FGameplayTag CurrentFrequencyTag;
	
};
