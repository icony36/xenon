// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XeGameModeBase.generated.h"

class UCharacterInfo;
/**
 * 
 */
UCLASS()
class XENON_API AXeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TObjectPtr<UCharacterInfo> CharacterInfo;
};
