// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "GDHUDWidget.generated.h"

struct FOnAttributeChangeData;
class UGDAbilitySystemComponent;
class UGDAttributeSet;

UCLASS()
class GASDEMO_API UGDHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void InitializeWithAbilitySystem(UGDAbilitySystemComponent* InAbilitySystemComponent, UGDAttributeSet* InAttributeSet);

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float Health = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float MaxHealth = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float Energy = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float MaxEnergy = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float Shield = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float MaxShield = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float Heat = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float MaxHeat = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float Sync = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Attributes")
	float MaxSync = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Frequency")
	FGameplayTag CurrentFrequencyTag;
	
	UPROPERTY(BlueprintReadOnly)
	float DashCoolDownRemainingPercent = 0.0f;
	
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle DashCoolDownTimer;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnHUDDataChanged();
	
	
protected:
	virtual void NativeDestruct() override;

private:
	void RefreshAttributeValues();
	void BindAttributeDelegates();
	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void HandleMaxHealthChanged(const FOnAttributeChangeData& Data);
	void HandleEnergyChanged(const FOnAttributeChangeData& Data);
	void HandleMaxEnergyChanged(const FOnAttributeChangeData& Data);
	void HandleShieldChanged(const FOnAttributeChangeData& Data);
	void HandleMaxShieldChanged(const FOnAttributeChangeData& Data);
	void HandleHeatChanged(const FOnAttributeChangeData& Data);
	void HandleMaxHeatChanged(const FOnAttributeChangeData& Data);
	void HandleSyncChanged(const FOnAttributeChangeData& Data);
	void HandleMaxSyncChanged(const FOnAttributeChangeData& Data);
	void HandleFrequencyTagChanged(const FGameplayTag Tag, int32 NewCount);
	void HandleDashCoolDownChanged(const FGameplayTag Tag, int32 NewCount);

	void GetDashCoolDownRemainingTime();
	
	UPROPERTY()
	TObjectPtr<UGDAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UGDAttributeSet> AttributeSet;
	
	
};
