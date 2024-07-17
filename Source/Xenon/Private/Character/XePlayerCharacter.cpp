// Copyright CoNY. All rights reserved.


#include "Character/XePlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/XePlayerState.h"

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
}

void AXePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Setup combat info for client.
	SetupCombatInfo();
}

void AXePlayerCharacter::SetupCombatInfo()
{
	AXePlayerState* XePlayerState = GetPlayerState<AXePlayerState>();
	checkf(XePlayerState, TEXT("XePlayerState is not valid in XePlayerCharacter."));
	
	// Set Ability System Component ability actor info.
	XePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(XePlayerState, this);

	// Set Character owned Ability System Component and Attribute Set.
	AbilitySystemComponent = XePlayerState->GetAbilitySystemComponent();
	AttributeSet = XePlayerState->GetAttributeSet();
}
