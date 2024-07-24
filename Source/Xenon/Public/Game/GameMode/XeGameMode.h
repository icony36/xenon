// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "XeGameMode.generated.h"

class UCharacterInfo;
class AXePlayerState;
class AXePlayerController;
class AXePlayerCharacter;
/**
 * 
 */
UCLASS()
class XENON_API AXeGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void EliminateCharacter(ACharacter* VictimCharacter, ACharacter* AttackerCharacter, AController* VictimController, AController* AttackerController);

	virtual void RespawnPlayer(ACharacter* CharacterToRespawn, AController* ControllerToRespawn);
	
	virtual void PlayerLeftGame(AXePlayerState* LeavingPlayerState);
	
	static void SendEXP(AActor* Recipient,  float InEXP);
	
	//~ Data
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TObjectPtr<UCharacterInfo> CharacterInfo;
	//~ end Data
	
protected:
	//~ Match State
	float CountdownTime = 0.f;
	//~ end Match State
};
