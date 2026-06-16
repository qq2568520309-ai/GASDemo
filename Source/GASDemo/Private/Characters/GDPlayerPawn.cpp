// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GDPlayerPawn.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include  "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAttributeSet.h"
#include "UI/GDHUDWidget.h"


// Sets default values
AGDPlayerPawn::AGDPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	//初始化所有组件
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneRoot;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SceneRoot);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneRoot);
	
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = 1000.0f;
	AbilitySystemComponent = CreateDefaultSubobject<UGDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	AttributeSet = CreateDefaultSubobject<UGDAttributeSet>(TEXT("AttributeSet"));
	
}

// Called when the game starts or when spawned
void AGDPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext,0);
			}
			
		}
		
		
	}
	//ASC初始化
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	GrantStartupAbilities();
	InitAttributeSet();
	AbilitySystemComponent->SetCurrentFrequencyTag(FGameplayTag::RequestGameplayTag("Frequency.Red"));
	CreateHUD();
	
}

void AGDPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AGDPlayerPawn::Move);
	EnhancedInputComponent->BindAction(ShiftFrequencyPressedAction,ETriggerEvent::Started,this,&AGDPlayerPawn::ShiftFrequency);
	EnhancedInputComponent->BindAction(DashPressedAction,ETriggerEvent::Started,this,&AGDPlayerPawn::Dash);
	EnhancedInputComponent->BindAction(PrimaryAction,ETriggerEvent::Started,this,&AGDPlayerPawn::Primary);
}



void AGDPlayerPawn::Move(const FInputActionValue& InputActionValue)
{
	FVector2D MoveVector = InputActionValue.Get<FVector2D>();
	if (!GetController())
	{
		return;
	}
	AddMovementInput(FVector::ForwardVector,MoveVector.X);

	AddMovementInput(FVector::RightVector,MoveVector.Y);
	
}

void AGDPlayerPawn::ShiftFrequency(const FInputActionValue& InputActionValue)
{
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Q pressed, trying Input.ShiftFrequency"));
	const FGameplayTag InputTag = FGameplayTag::RequestGameplayTag("Input.ShiftFrequency");
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
	
}

void AGDPlayerPawn::Dash(const FInputActionValue& InputActionValue)
{
	if (AbilitySystemComponent==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is nullptr"));
		return;
	}
	
	const FGameplayTag InputTag = FGameplayTag::RequestGameplayTag("Input.Dash");
	
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
	
}

void AGDPlayerPawn::Primary(const FInputActionValue& InputActionValue)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Input.Primary");
	if (Tag.IsValid())
	{
		AbilitySystemComponent->AbilityInputTagPressed(Tag);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Tag in InValid"));
	}
	
}

void AGDPlayerPawn::InitAttributeSet()
{
	if (AbilitySystemComponent && AttributeInitEffect)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttributeInitEffect,1.0f,ContextHandle);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	
	
}

void AGDPlayerPawn::CreateHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerController->IsLocalController() || !HUDWidgetClass)
	{
		return;
	}

	HUDWidget = CreateWidget<UGDHUDWidget>(PlayerController, HUDWidgetClass);
	if (!HUDWidget)
	{
		return;
	}

	HUDWidget->InitializeWithAbilitySystem(AbilitySystemComponent, AttributeSet);
	HUDWidget->AddToViewport();
}

void AGDPlayerPawn::GrantStartupAbilities()
{
	if (AbilitySystemComponent==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is nullptr"));
		return;
	}
	
	AbilitySystemComponent->AddCharacterAbilities(StartupAbilities);
}

UAbilitySystemComponent* AGDPlayerPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

