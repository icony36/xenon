// Copyright CoNY. All rights reserved.


#include "Game/GameMode/KillRaceGameMode.h"

#include "AbilitySystem/Data/CharacterInfo.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"

void AKillRaceGameMode::PlayerEliminated(ACharacter* VictimCharacter, ACharacter* AttackerCharacter,
	AController* VictimController, AController* AttackerController)
{
	if (VictimCharacter->Implements<UCombatInterface>())
	{
		// Kill victim character.
		ICombatInterface::Execute_Die(VictimCharacter);

		// Get victim level.
		const int32 VictimLevel = ICombatInterface::Execute_GetCombatLevel(VictimCharacter);

		// Get victim character tag.
		const FGameplayTag VictimTag = ICombatInterface::Execute_GetCharacterTag(VictimCharacter);

		// Get victim character properties.
		const FCharacterProperties Props = CharacterInfo->GetCharacterProperties(VictimTag);

		// Get EXP reward at the level.
		const float EXPReward = Props.EXPReward.GetValueAtLevel(VictimLevel);

		// Send EXP to attacker character.
		SendEXP(AttackerCharacter, EXPReward);
	}
}

void AKillRaceGameMode::PlayerRespawn(ACharacter* CharacterToSpawn, AController* ControllerToSpawn)
{
	
}

void AKillRaceGameMode::PlayerLeftGame(AXePlayerState* LeavingPlayerState)
{
	
}
