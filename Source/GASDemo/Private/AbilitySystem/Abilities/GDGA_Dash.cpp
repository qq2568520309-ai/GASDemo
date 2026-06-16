// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDGA_Dash.h"

#include "AbilitySystemComponent.h"
#include "Characters/GDPlayerPawn.h"


UGDGA_Dash::UGDGA_Dash()
{
	InputTag = FGameplayTag::RequestGameplayTag("Input.Dash");
}

void UGDGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	if (!CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,false,true);
		return;
	}
	AGDPlayerPawn* Pawn = Cast<AGDPlayerPawn>(GetAvatarActorFromActorInfo());
	if (Pawn)
	{
		if (InVulnerableEffect)
		{
			FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			ContextHandle.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
			InVulnerableEffect,
			1.0f,
			ContextHandle
			);
			if (SpecHandle.IsValid())
			{
				GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
			
		}
		
		
		FVector DirVector = Pawn->GetLastMovementInputVector();
		
		if (DirVector.IsNearlyZero())
		{
			DirVector = Pawn->GetActorForwardVector().GetSafeNormal();
			Pawn->SetActorLocation(Pawn->GetActorLocation()+DirVector*Pawn->DashDistance,true);
			EndAbility(Handle,ActorInfo,ActivationInfo,false,false);
			return;
		}
		Pawn->AddActorWorldOffset(DirVector*Pawn->DashDistance,true);
		EndAbility(Handle,ActorInfo,ActivationInfo,false,false);
	}
	
}
