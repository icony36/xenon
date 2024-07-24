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
#include "Game/GameMode/XeGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
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
	
	// Initialize player on server.
	InitializePlayer();

	// Add startup abilities.
	AddStartupAbilities();
}

void AXePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize player on client.
	InitializePlayer();
}

int32 AXePlayerCharacter::GetCombatLevel_Implementation()
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	return XePlayerState->GetCombatLevel();
}

int32 AXePlayerCharacter::FindCombatLevelWithEXP_Implementation(int32 InEXP)
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
    
	return XePlayerState->GetLevelInfo()->FindLevelWithEXP(InEXP);
}

int32 AXePlayerCharacter::GetEXP_Implementation()
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
    
	return XePlayerState->GetExperience();
}

int32 AXePlayerCharacter::GetSkillPoint_Implementation()
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	return XePlayerState->GetSkillPoint();
}

FLevelUpProperties AXePlayerCharacter::GetLevelUpProperties_Implementation(int32 Level)
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
	
	return XePlayerState->GetLevelInfo()->LevelUpInformation[Level];
}

void AXePlayerCharacter::AddToCombatLevel_Implementation(int32 InCombatLevel)
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToCombatLevel(InCombatLevel);
}

void AXePlayerCharacter::AddToEXP_Implementation(int32 InEXP)
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToExperience(InEXP);
}

void AXePlayerCharacter::AddToSkillPoint_Implementation(int32 InSkillPoint)
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToSkillPoint(InSkillPoint);
}

void AXePlayerCharacter::LevelUp_Implementation()
{
	MulticastPlayLevelUpEffects();
}

void AXePlayerCharacter::MulticastHandleDeath_Implementation(float RespawnTime)
{
	Super::MulticastHandleDeath_Implementation(RespawnTime);

	if (RespawnTime >= 0)
	{
		// Start respawn cooldown.
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &AXePlayerCharacter::RespawnTimerFinished, RespawnTime);
	}
	
}

void AXePlayerCharacter::InitializePlayer()
{
	// Setup combat info.
	SetupCombatInfo();
	
	// Setup Overhead Widget.
	SetupOverheadWidget();
}

void AXePlayerCharacter::SetupCombatInfo()
{
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
	
	// Set Ability System Component ability actor info.
	XePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(XePlayerState, this);

	// Set Character owned Ability System Component and Attribute Set.
	if (!IsValid(XeAbilitySystemComponent)) XeAbilitySystemComponent = CastChecked<UXeAbilitySystemComponent>(XePlayerState->GetAbilitySystemComponent());
	if (!IsValid(XeAttributeSet)) XeAttributeSet = CastChecked<UXeAttributeSet>(XePlayerState->GetAttributeSet());
	

	// Bind callbacks to attributes changed.
	BindCallbacksToDependencies();
	
	// Initialize default Attributes.
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	
	// Setup HUD on local controlled Character.
	XePlayerController = XePlayerController == nullptr ?  Cast<AXePlayerController>(GetController()) : XePlayerController;
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
	// Ensure the widget is initialized.
	OverheadWidget->InitWidget();
	
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
	
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
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
	XePlayerState = XePlayerState == nullptr ? GetPlayerState<AXePlayerState>() : XePlayerState;
	XePlayerState->OnCombatLevelChangedDelegate.AddLambda(
		[this](const int32 NewLevel)
		{
			OnCombatLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
}

void AXePlayerCharacter::RespawnTimerFinished()
{
	XeGameMode = XeGameMode == nullptr ? Cast<AXeGameMode>(UGameplayStatics::GetGameMode(this)) : XeGameMode;
	if (XeGameMode)
	{
		XeGameMode->RespawnPlayer(this, Controller);
	}
}


void AXePlayerCharacter::MulticastPlayLevelUpEffects_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}
