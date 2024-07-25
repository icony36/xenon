// Copyright CoNY. All rights reserved.


#include "Game/GameMode/KillRaceGameMode.h"

#include "AbilitySystem/Data/CharacterInfo.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/CombatInterface.h"
#include "Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

void AKillRaceGameMode::EliminateCharacter(ACharacter* VictimCharacter, ACharacter* AttackerCharacter,
                                        AController* VictimController, AController* AttackerController)
{
	if (VictimCharacter->Implements<UCombatInterface>())
	{
		// Kill victim character.
		if (VictimCharacter->Implements<UPlayerInterface>())
		{
			ICombatInterface::Execute_Die(VictimCharacter, PlayerRespawnTime);
		}
		else
		{
			// For non-player character, execute Die with no respawn by passing -1.f.
			ICombatInterface::Execute_Die(VictimCharacter, -1.f);
		}
		
		if (AttackerCharacter->Implements<UPlayerInterface>())
		{
			// Get victim level.
			const int32 VictimLevel = ICombatInterface::Execute_GetCombatLevel(VictimCharacter);

			// Get victim character tag.
			const FGameplayTag VictimTag = ICombatInterface::Execute_GetCharacterTag(VictimCharacter);

			// Get victim character properties.
			const FCharacterProperties Props = CharacterInfo->GetCharacterProperties(VictimTag);

			// Get EXP reward at the level.
			const float EXPReward = Props.EXPReward.GetValueAtLevel(VictimLevel);
			
			// Send EXP to attacker player character.
			SendEXP(AttackerCharacter, EXPReward);
		}
	}
}

void AKillRaceGameMode::RespawnPlayer(ACharacter* CharacterToRespawn, AController* ControllerToRespawn)
{
	// Reset player controller and destroy character.
	if (CharacterToRespawn)
	{
		CharacterToRespawn->Reset();
		CharacterToRespawn->Destroy();
	}

	// Respawn player
	if (ControllerToRespawn)
	{
		// Get all Player Start in the map.
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	
		// Random an index.
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
	
		// Randomly respawn at one of the Player Start.
		CharacterToRespawn->SetActorLocation(PlayerStarts[Selection]->GetActorLocation());
		RestartPlayerAtPlayerStart(ControllerToRespawn, PlayerStarts[Selection]); // * spawn actor is replicated by default
	}
}

void AKillRaceGameMode::PlayerLeftGame(AXePlayerState* LeavingPlayerState)
{
	
}
