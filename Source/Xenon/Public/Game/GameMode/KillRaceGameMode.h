// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameMode/XeGameMode.h"
#include "KillRaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API AKillRaceGameMode : public AXeGameMode
{
	GENERATED_BODY()

public:
	virtual void EliminateCharacter(ACharacter* VictimCharacter, ACharacter* AttackerCharacter, AController* VictimController, AController* AttackerController) override;

	virtual void RespawnPlayer(ACharacter* CharacterToRespawn, AController* ControllerToRespawn) override;

	virtual void PlayerLeftGame(AXePlayerState* LeavingPlayerState) override; // Called by ServerLeaveGame() in XePlayerCharacter

protected:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float PlayerRespawnTime = 1.f;
};
