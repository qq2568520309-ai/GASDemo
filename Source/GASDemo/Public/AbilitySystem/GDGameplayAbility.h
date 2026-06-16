// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GDGameplayAbility.generated.h"


UCLASS()
class GASDEMO_API UGDGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	public:
	UGDGameplayAbility();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	FGameplayTag InputTag;
	
};
