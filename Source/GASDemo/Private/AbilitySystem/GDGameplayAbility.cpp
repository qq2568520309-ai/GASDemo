// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDGameplayAbility.h"

UGDGameplayAbility::UGDGameplayAbility()
{
	//每个实例拥有一份GA数据
	InstancingPolicy  = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
