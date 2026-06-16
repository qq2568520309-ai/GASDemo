// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDGameplayAbility.h"

UGDAbilitySystemComponent::UGDAbilitySystemComponent()
{
}

void UGDAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	UE_LOG(LogTemp, Warning, TEXT("AddCharacterAbilities Count: %d"), StartupAbilities.Num());

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
	{
		if (!AbilityClass)
		{
			continue;
		}
		

		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);

		if (const UGDGameplayAbility* GDAbility = Cast<UGDGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(GDAbility->InputTag);
		}

		GiveAbility(AbilitySpec);
	}
}

void UGDAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	//作用域锁，防止执行代码过程中Ability发生更改导致的数组越界等问题
	//一般在遍历Ability的时候添加
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{


		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UGDAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}


void UGDAbilitySystemComponent::SetCurrentFrequencyTag(const FGameplayTag FrequencyTag)
{
	if (CurrentFrequencyTag.IsValid())
	{
		RemoveLooseGameplayTag(CurrentFrequencyTag);
	}
	CurrentFrequencyTag = FrequencyTag;
	if (CurrentFrequencyTag.IsValid())
	{
		AddLooseGameplayTag(CurrentFrequencyTag);
	}
	
}

bool UGDAbilitySystemComponent::GetCooldownRemainingForTag(const FGameplayTag& CooldownTag, float& OutRemainingTime,
	float& OutDuration) const
{
	OutRemainingTime = 0.f;
	OutDuration = 0.f;

	if (!CooldownTag.IsValid())
	{
		return false;
	}

	FGameplayTagContainer CooldownTags;
	CooldownTags.AddTag(CooldownTag);

	FGameplayEffectQuery Query =
		FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
	
	TArray<TPair<float, float>> TimeRemainingAndDuration =
		GetActiveEffectsTimeRemainingAndDuration(Query);

	if (TimeRemainingAndDuration.Num() <= 0)
	{
		return false;
	}
	
	float LongestRemainingTime = 0.f;
	float MatchingDuration = 0.f;

	for (const TPair<float, float>& Pair : TimeRemainingAndDuration)
	{
		const float RemainingTime = Pair.Key;
		const float Duration = Pair.Value;

		if (RemainingTime > LongestRemainingTime)
		{
			LongestRemainingTime = RemainingTime;
			MatchingDuration = Duration;
		}
	}

	OutRemainingTime = LongestRemainingTime;
	OutDuration = MatchingDuration;

	return OutRemainingTime > 0.f;
	
}

FGameplayTag UGDAbilitySystemComponent::GetCurrentFrequencyTag() const
{
	return CurrentFrequencyTag;
}
