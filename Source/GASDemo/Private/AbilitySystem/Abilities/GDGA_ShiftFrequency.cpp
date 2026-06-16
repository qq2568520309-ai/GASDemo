// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDGA_ShiftFrequency.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"


UGDGA_ShiftFrequency::UGDGA_ShiftFrequency()
{
	InputTag = FGameplayTag::RequestGameplayTag("Input.ShiftFrequency");
}

void UGDGA_ShiftFrequency::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("ShiftFrequency Ability Activated"));
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,true);
		return;
	}
	
	UGDAbilitySystemComponent* GDASC = Cast<UGDAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!GDASC)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FGameplayTag RedTag = FGameplayTag::RequestGameplayTag(TEXT("Frequency.Red"));
	const FGameplayTag BlueTag = FGameplayTag::RequestGameplayTag(TEXT("Frequency.Blue"));
	const FGameplayTag GreenTag = FGameplayTag::RequestGameplayTag(TEXT("Frequency.Green"));

	const FGameplayTag CurrentTag = GDASC->GetCurrentFrequencyTag();
	
	if (CurrentTag == RedTag)
	{
		GDASC->SetCurrentFrequencyTag(BlueTag);
	}
	else if (CurrentTag == BlueTag)
	{
		GDASC->SetCurrentFrequencyTag(GreenTag);
	}
	else
	{
		GDASC->SetCurrentFrequencyTag(RedTag);
	}

	//最后面那个参数为false表示正常结束
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	
}
