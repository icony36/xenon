// Copyright CoNY. All rights reserved.


#include "Character/XePlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/XePlayerController.h"
#include "Player/XePlayerState.h"
#include "UI/HUD/XeHUD.h"
#include "UI/Widget/XeUserWidget.h"

AXePlayerCharacter::AXePlayerCharacter()
{
	// Setup Camera Component.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1400.f;
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("CamaraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->FieldOfView = 70.f;
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	
	// Setup Movement Component.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f); // speed of character rotate
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AXePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Setup combat info for server.
	SetupCombatInfo();
	
	// Add startup abilities.
	AddStartupAbilities();
}

void AXePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Setup combat info for client.
	SetupCombatInfo();
}

int32 AXePlayerCharacter::GetCombatLevel_Implementation()
{
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	return XePlayerState->GetCombatLevel();
}

void AXePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupOverheadWidget();
}

void AXePlayerCharacter::SetupCombatInfo()
{
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
	
	// Set Ability System Component ability actor info.
	XePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(XePlayerState, this);

	// Set Character owned Ability System Component and Attribute Set.
	XeAbilitySystemComponent = CastChecked<UXeAbilitySystemComponent>(XePlayerState->GetAbilitySystemComponent());
	XeAttributeSet = CastChecked<UXeAttributeSet>(XePlayerState->GetAttributeSet());

	// Initialize default Attributes.
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	
	// Setup HUD on local controlled Character.
	AXePlayerController* XePlayerController = Cast<AXePlayerController>(GetController());
	if (XePlayerController != nullptr) // * character might not have Player Controller (non locally controlled character)
	{
		if (AXeHUD* XeHUD = XePlayerController->GetHUD<AXeHUD>())
		{
			XeHUD->InitializeOverlay(XePlayerController, XePlayerState, XeAbilitySystemComponent, XeAttributeSet);
		}
	}

	// Bind delegates for Attribute changed.
	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	);
	
	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	// Bind delegates for Level changed.
	XePlayerState->OnCombatLevelChangedDelegate.AddLambda(
		[this](const int32 NewLevel)
		{
			OnCombatLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
}

void AXePlayerCharacter::SetupOverheadWidget()
{
	// Setup Overhead Widget Controller.
	if (UXeUserWidget* XeOverheadWidget = Cast<UXeUserWidget>(OverheadWidget->GetUserWidgetObject()))
	{
		XeOverheadWidget->SetWidgetController(this);
	}
	
	// Broadcast initial values for Overhead Widget
	OnHealthChangedDelegate.Broadcast(XeAttributeSet->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(XeAttributeSet->GetMaxHealth());
	OnManaChangedDelegate.Broadcast(XeAttributeSet->GetMana());
	OnMaxManaChangedDelegate.Broadcast(XeAttributeSet->GetMaxMana());
	
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	OnCombatLevelChangedDelegate.Broadcast(XePlayerState->GetCombatLevel());
}
