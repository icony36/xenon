// Copyright CoNY. All rights reserved.


#include "Player/XePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "Character/XePlayerCharacter.h"
#include "Input/XeInputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"

AXePlayerController::AXePlayerController()
{
	bReplicates = true;
}

void AXePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set mouse cursor.
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Set input mode.
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AXePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context to Enhanced Input Subsystem.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	checkf(InputMappingContext, TEXT("InputMappingContext is not set in BP_XePlayerController."));
	Subsystem->AddMappingContext(InputMappingContext, 0);

	// Connect User Settings to Input Mapping Context.
	if (UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings())
	{
		if (!UserSettings->IsMappingContextRegistered(InputMappingContext))
		{
			UserSettings->RegisterInputMappingContext(InputMappingContext);
		}
	}
	

	// Bind Input Actions.
	UXeInputComponent* XeInputComponent = CastChecked<UXeInputComponent>(InputComponent);
	XeInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AXePlayerController::Move);
	XeInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AXePlayerController::MoveCompleted);
	XeInputComponent->BindAbilityActions(InputTagConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}


// ReSharper disable once CppMemberFunctionMayBeConst
void AXePlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Retrieve vector 2D values from input.
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	if (AXePlayerCharacter* ControlledPawn = GetPawn<AXePlayerCharacter>()) // * move might be called before Pawn is valid
	{
		ControlledPawn->AddMovementInput(FVector::RightVector, InputAxisVector.X);
		ControlledPawn->AddMovementInput(FVector::ForwardVector, InputAxisVector.Y);

		ControlledPawn->MoveInputValue.X = InputAxisVector.X;
		ControlledPawn->MoveInputValue.Y = InputAxisVector.Y;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AXePlayerController::MoveCompleted(const FInputActionValue& InputActionValue)
{
	// Retrieve vector 2D values from input.
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	
	if (AXePlayerCharacter* ControlledPawn = GetPawn<AXePlayerCharacter>()) // * move might be called before Pawn is valid
	{
		ControlledPawn->MoveInputValue.X = InputAxisVector.X;
		ControlledPawn->MoveInputValue.Y = InputAxisVector.Y;
	}
}

void AXePlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() == nullptr) return;

	GetAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
}

void AXePlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() == nullptr) return;
	
	GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}

void AXePlayerController::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() == nullptr) return;
	
	GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
}

UXeAbilitySystemComponent* AXePlayerController::GetAbilitySystemComponent()
{
	if (XeAbilitySystemComponent == nullptr)
	{
		XeAbilitySystemComponent = Cast<UXeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return XeAbilitySystemComponent;
}
