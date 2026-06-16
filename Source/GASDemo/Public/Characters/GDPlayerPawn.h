// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "GDPlayerPawn.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UGDAttributeSet;
class UFloatingPawnMovement;
class USpringArmComponent;
class UCameraComponent;
class UGDAbilitySystemComponent;
class UGameplayEffect;
class UGDHUDWidget;
class UGameplayAbility;
UCLASS()
class GASDEMO_API AGDPlayerPawn : public APawn,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGDPlayerPawn();
	virtual void BeginPlay() override;

protected:
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UGDAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGDAttributeSet> AttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ShiftFrequencyPressedAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DashPressedAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> PrimaryAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> AttributeInitEffect;
	
	
	
	//角色开始所拥有的能力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UGDHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UGDHUDWidget> HUDWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashDistance = 500.0f;
	
	void Move(const FInputActionValue& InputActionValue);
	
	void ShiftFrequency(const FInputActionValue& InputActionValue);
	
	void Dash(const FInputActionValue& InputActionValue);
	
	void Primary(const FInputActionValue& InputActionValue);
	
	void InitAttributeSet();

	void CreateHUD();
	
	void GrantStartupAbilities();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	
};
