// Copyright CoNY. All rights reserved.


#include "Player/XePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

	// Set input mode so the player can use input from keyboard and mouse and able to effect UI.
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AXePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set Input Mapping Context.
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	checkf(InputMappingContext, TEXT("InputMappingContext is not set in Player Controller."));
	Subsystem->AddMappingContext(InputMappingContext, 0);

	// Bind Input Actions.
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AXePlayerController::Move);
}

void AXePlayerController::Move(const FInputActionValue& InputActionValue)
{
	// Retrieve vector 2D value from input.
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	if (APawn* ControlledPawn = GetPawn<APawn>()) // move might be called before Pawn is valid.
	{
		ControlledPawn->AddMovementInput(FVector::RightVector, InputAxisVector.X);
		ControlledPawn->AddMovementInput(FVector::ForwardVector, InputAxisVector.Y);
	}
}
