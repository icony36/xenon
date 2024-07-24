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
	virtual void PlayerEliminated(ACharacter* VictimCharacter, ACharacter* AttackerCharacter, AController* VictimController, AController* AttackerController) override;
};
