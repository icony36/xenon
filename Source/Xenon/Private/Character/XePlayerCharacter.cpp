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
	InitializeCharacter();

	// Add startup abilities.
	AddStartupAbilities();
}

void AXePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize player on client.
	InitializeCharacter();
}

int32 AXePlayerCharacter::GetCombatLevel_Implementation()
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	return XePlayerState->GetCombatLevel();
}

int32 AXePlayerCharacter::FindCombatLevelWithEXP_Implementation(int32 InEXP)
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
    
	return XePlayerState->GetLevelInfo()->FindLevelWithEXP(InEXP);
}

int32 AXePlayerCharacter::GetEXP_Implementation()
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
    
	return XePlayerState->GetExperience();
}

int32 AXePlayerCharacter::GetSkillPoint_Implementation()
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	return XePlayerState->GetSkillPoint();
}

FLevelUpProperties AXePlayerCharacter::GetLevelUpProperties_Implementation(int32 Level)
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
	
	return XePlayerState->GetLevelInfo()->LevelUpInformation[Level];
}

void AXePlayerCharacter::AddToCombatLevel_Implementation(int32 InCombatLevel)
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToCombatLevel(InCombatLevel);
}

void AXePlayerCharacter::AddToEXP_Implementation(int32 InEXP)
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToExperience(InEXP);
}

void AXePlayerCharacter::AddToSkillPoint_Implementation(int32 InSkillPoint)
{
	if (!IsValid(XePlayerState)) XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));

	XePlayerState->AddToSkillPoint(InSkillPoint);
}

void AXePlayerCharacter::LevelUp_Implementation()
{
	MulticastPlayLevelUpEffects();
}

void AXePlayerCharacter::InitializeCharacter()
{
	// Set Character owned Player State;
	XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
	
	// Set Ability System Component ability actor info.
	XePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(XePlayerState, this);

	// Set Character owned Ability System Component and Attribute Set.
	XeAbilitySystemComponent = CastChecked<UXeAbilitySystemComponent>(XePlayerState->GetAbilitySystemComponent());
	XeAttributeSet = CastChecked<UXeAttributeSet>(XePlayerState->GetAttributeSet());
	
	// Check if is first time spawning in the game.
	if (XePlayerState->bIsFirstSpawn)
	{
		HandleFirstSpawn();
	}
	else
	{
		HandleRespawn();
	}

	// Setup Overhead Widget.
	SetupOverheadWidget();
}

void AXePlayerCharacter::HandleFirstSpawn() const
{
	if (HasAuthority())
	{
		// Initialize all default Attributes.
		InitializeDefaultAttributes();
	}

	// Setup HUD on local controlled Character.
	SetupHUD();

	// The following spawn will be respawning.
	XePlayerState->bIsFirstSpawn = false;
}

void AXePlayerCharacter::HandleRespawn() const
{
	if (HasAuthority())
	{
		// Reset health and mana for respawn.
		ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	}
}

void AXePlayerCharacter::SetupHUD() const
{
	if (AXePlayerController* XePlayerController = Cast<AXePlayerController>(GetController())) // * only server and locally controller character have HUD
	{
		if (AXeHUD* XeHUD = XePlayerController->GetHUD<AXeHUD>()) // * only locally controller character has HUD
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

	// Bind callbacks to Attributes changed.
	BindCallbacksToDependencies();
	
	// Broadcast initial values for Overhead Widget
	BroadcastInitialValues();
}

void AXePlayerCharacter::BindCallbacksToDependencies()
{
	// Bind delegates for Attributes changed.
	OnHealthChangedDelegateHandle = XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	);
	
	OnMaxHealthChangedDelegateHandle = XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	OnManaChangedDelegateHandle = XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnManaChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	OnMaxManaChangedDelegateHandle = XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	// Bind delegates for Level changed.
	OnCombatLevelChangedDelegateHandle = XePlayerState->OnCombatLevelChangedDelegate.AddLambda(
		[this](const int32 NewLevel)
		{
			OnCombatLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
}

void AXePlayerCharacter::UnbindCallbacksFromDependencies()
{
	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetHealthAttribute()).Remove(OnHealthChangedDelegateHandle);
	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMaxHealthAttribute()).Remove(OnMaxHealthChangedDelegateHandle);
	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetManaAttribute()).Remove(OnManaChangedDelegateHandle);
	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMaxManaAttribute()).Remove(OnMaxManaChangedDelegateHandle);
	XePlayerState->OnCombatLevelChangedDelegate.Remove(OnCombatLevelChangedDelegateHandle);
}

void AXePlayerCharacter::BroadcastInitialValues()
{
	OnHealthChangedDelegate.Broadcast(XeAttributeSet->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(XeAttributeSet->GetMaxHealth());
	OnManaChangedDelegate.Broadcast(XeAttributeSet->GetMana());
	OnMaxManaChangedDelegate.Broadcast(XeAttributeSet->GetMaxMana());
	OnCombatLevelChangedDelegate.Broadcast(XePlayerState->GetCombatLevel());
}

void AXePlayerCharacter::MulticastPlayLevelUpEffects_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}

void AXePlayerCharacter::Die_Implementation(float RespawnTime)
{
	Super::Die_Implementation(RespawnTime);

	TopDownCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AXePlayerCharacter::RespawnTimerFinished()
{
	if (AXeGameMode* XeGameMode = Cast<AXeGameMode>(UGameplayStatics::GetGameMode(this))) // * only server have game mode
	{
		XeGameMode->RespawnPlayer(this, Controller);
	}
}
