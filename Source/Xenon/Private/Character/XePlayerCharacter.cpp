// Copyright CoNY. All rights reserved.


#include "Character/XePlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "AbilitySystem/Data/LevelInfo.h"
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


	// Setup Combat
	CharacterTag = FXeGameplayTags::Get().Character_Player;

	
	// Level Up Effect
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void AXePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Setup combat info for server.
	SetupCombatInfo();

	// Setup Overhead Widget for server (non-server controlled character).
	SetupOverheadWidget();

	// Add startup abilities.
	AddStartupAbilities();
}

void AXePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Setup combat info for client.
	SetupCombatInfo();
	
	// Setup Overhead Widget for client.
	SetupOverheadWidget();
}

int32 AXePlayerCharacter::GetCombatLevel_Implementation()
{
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	return XePlayerState->GetCombatLevel();
}

int32 AXePlayerCharacter::FindCombatLevelWithEXP_Implementation(int32 InEXP) const
{
	const AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
    
	return XePlayerState->GetLevelInfo()->FindLevelWithEXP(InEXP);
}

int32 AXePlayerCharacter::GetEXP_Implementation() const
{
	const AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
    
	return XePlayerState->GetExperience();
}

int32 AXePlayerCharacter::GetSkillPoint_Implementation() const
{
	const AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	return XePlayerState->GetSkillPoint();
}

FLevelUpProperties AXePlayerCharacter::GetLevelUpProperties_Implementation(int32 Level) const
{
	const AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
	
	return XePlayerState->GetLevelInfo()->LevelUpInformation[Level];
}

void AXePlayerCharacter::AddToCombatLevel_Implementation(int32 InCombatLevel)
{
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToCombatLevel(InCombatLevel);
}

void AXePlayerCharacter::AddToEXP_Implementation(int32 InEXP)
{
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToExperience(InEXP);
}

void AXePlayerCharacter::AddToSkillPoint_Implementation(int32 InSkillPoint)
{
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToSkillPoint(InSkillPoint);
}

void AXePlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpEffects();
}

void AXePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Setup Overhead Widget for server controlled character only (Overhead Widget is not ready yet in Possessed for server controlled character).
	if (GetLocalRole() == ROLE_Authority && IsLocallyControlled())
	{
		SetupOverheadWidget();
	}
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

	// Bind callbacks to attributes changed.
	BindCallbacksToDependencies();
	
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

void AXePlayerCharacter::BindCallbacksToDependencies()
{
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
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	
	XePlayerState->OnCombatLevelChangedDelegate.AddLambda(
		[this](const int32 NewLevel)
		{
			OnCombatLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
}

void AXePlayerCharacter::MulticastLevelUpEffects_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}
