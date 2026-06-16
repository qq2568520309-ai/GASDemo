// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GDHUDWidget.h"

#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAttributeSet.h"

void UGDHUDWidget::InitializeWithAbilitySystem(UGDAbilitySystemComponent* InAbilitySystemComponent, UGDAttributeSet* InAttributeSet)
{
	AbilitySystemComponent = InAbilitySystemComponent;
	AttributeSet = InAttributeSet;

	RefreshAttributeValues();
	BindAttributeDelegates();
	OnHUDDataChanged();
}

void UGDHUDWidget::NativeDestruct()
{
	AbilitySystemComponent = nullptr;
	AttributeSet = nullptr;

	Super::NativeDestruct();
}

void UGDHUDWidget::RefreshAttributeValues()
{
	if (!AbilitySystemComponent || !AttributeSet)
	{
		return;
	}

	Health = AttributeSet->GetHealth();
	MaxHealth = AttributeSet->GetMaxHealth();
	Energy = AttributeSet->GetEnergy();
	MaxEnergy = AttributeSet->GetMaxEnergy();
	Shield = AttributeSet->GetShield();
	MaxShield = AttributeSet->GetMaxShield();
	Heat = AttributeSet->GetHeat();
	MaxHeat = AttributeSet->GetMaxHeat();
	Sync = AttributeSet->GetSync();
	MaxSync = AttributeSet->GetMaxSync();
	CurrentFrequencyTag = AbilitySystemComponent->GetCurrentFrequencyTag();
}

void UGDHUDWidget::BindAttributeDelegates()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetHealthAttribute()).AddUObject(this, &UGDHUDWidget::HandleHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UGDHUDWidget::HandleMaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetEnergyAttribute()).AddUObject(this, &UGDHUDWidget::HandleEnergyChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetMaxEnergyAttribute()).AddUObject(this, &UGDHUDWidget::HandleMaxEnergyChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetShieldAttribute()).AddUObject(this, &UGDHUDWidget::HandleShieldChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetMaxShieldAttribute()).AddUObject(this, &UGDHUDWidget::HandleMaxShieldChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetHeatAttribute()).AddUObject(this, &UGDHUDWidget::HandleHeatChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetMaxHeatAttribute()).AddUObject(this, &UGDHUDWidget::HandleMaxHeatChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetSyncAttribute()).AddUObject(this, &UGDHUDWidget::HandleSyncChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGDAttributeSet::GetMaxSyncAttribute()).AddUObject(this, &UGDHUDWidget::HandleMaxSyncChanged);

	const FGameplayTag RedFrequencyTag = FGameplayTag::RequestGameplayTag(TEXT("Frequency.Red"));
	const FGameplayTag BlueFrequencyTag = FGameplayTag::RequestGameplayTag(TEXT("Frequency.Blue"));
	const FGameplayTag GreenFrequencyTag = FGameplayTag::RequestGameplayTag(TEXT("Frequency.Green"));
	const FGameplayTag DashCoolDownTag = FGameplayTag::RequestGameplayTag(TEXT("CoolDown.Dash"));
	
	AbilitySystemComponent->RegisterGameplayTagEvent(RedFrequencyTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGDHUDWidget::HandleFrequencyTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(BlueFrequencyTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGDHUDWidget::HandleFrequencyTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(GreenFrequencyTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UGDHUDWidget::HandleFrequencyTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(DashCoolDownTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UGDHUDWidget::HandleDashCoolDownChanged);
}

void UGDHUDWidget::HandleHealthChanged(const FOnAttributeChangeData& Data)
{
	Health = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	MaxHealth = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleEnergyChanged(const FOnAttributeChangeData& Data)
{
	Energy = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleMaxEnergyChanged(const FOnAttributeChangeData& Data)
{
	MaxEnergy = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleShieldChanged(const FOnAttributeChangeData& Data)
{
	Shield = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleMaxShieldChanged(const FOnAttributeChangeData& Data)
{
	MaxShield = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleHeatChanged(const FOnAttributeChangeData& Data)
{
	Heat = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleMaxHeatChanged(const FOnAttributeChangeData& Data)
{
	MaxHeat = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleSyncChanged(const FOnAttributeChangeData& Data)
{
	Sync = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleMaxSyncChanged(const FOnAttributeChangeData& Data)
{
	MaxSync = Data.NewValue;
	OnHUDDataChanged();
}

void UGDHUDWidget::HandleFrequencyTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		CurrentFrequencyTag = Tag;
		OnHUDDataChanged();
	}
}

void UGDHUDWidget::HandleDashCoolDownChanged(const FGameplayTag Tag, int32 NewCount)
{
	GetWorld()->GetTimerManager().SetTimer(
	DashCoolDownTimer,
	this,
	&UGDHUDWidget::GetDashCoolDownRemainingTime,
	0.1,
	true
	);
}

void UGDHUDWidget::GetDashCoolDownRemainingTime()
{
	float DashCoolDownRemaining = 0.0f;
	float DashCoolDownDuration = 0.0f;
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag("CoolDown.Dash");
	AbilitySystemComponent->GetCooldownRemainingForTag(Tag,DashCoolDownRemaining,DashCoolDownDuration);
	if (FMath::Abs(DashCoolDownDuration) <= 0.01)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashCoolDownTimer);
		DashCoolDownRemainingPercent = 0.0f;
		OnHUDDataChanged();
		return;
	}
	DashCoolDownRemainingPercent = DashCoolDownRemaining/DashCoolDownDuration;
	OnHUDDataChanged();
}
