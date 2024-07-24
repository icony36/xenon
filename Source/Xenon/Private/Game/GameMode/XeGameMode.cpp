// Copyright CoNY. All rights reserved.


#include "Game/GameMode/XeGameMode.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "XeGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"


struct FXeGameplayTags;

void AXeGameMode::EliminatePlayer(ACharacter* VictimCharacter, ACharacter* AttackerCharacter, AController* VictimController,
                                   AController* AttackerController)
{
	// Implement in child class.
}

void AXeGameMode::RespawnPlayer(ACharacter* CharacterToSpawn, AController* ControllerToSpawn)
{
	// Implement in child class.
}

void AXeGameMode::PlayerLeftGame(AXePlayerState* LeavingPlayerState)
{
	// Implement in child class.
}

void AXeGameMode::SendEXP(AActor* Recipient, float InEXP)
{
	// Setup payload to send (EXP tag, EXP reward amount).
	const FXeGameplayTags& GameplayTags = FXeGameplayTags::Get();
	FGameplayEventData Payload;
	Payload.EventTag = GameplayTags.Attribute_Meta_IncomingEXP;
	Payload.EventMagnitude = InEXP;

	// Send EXP to target actor through Gameplay Event.
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Recipient, GameplayTags.Attribute_Meta_IncomingEXP, Payload);
}
