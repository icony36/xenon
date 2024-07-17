// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/XeCharacter.h"
#include "XeAICharacter.generated.h"

class UAttributeSet;
/**
 * 
 */
UCLASS()
class XENON_API AXeAICharacter : public AXeCharacter
{
	GENERATED_BODY()

public:
	AXeAICharacter();

	//~ Combat Interface
	virtual int32 GetCombatLevel_Implementation() override;
	//~ end Combat Interface
	
protected:
	virtual void BeginPlay() override;
	
	
	//~ Combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI Character Defaults") // * this is not replicated as we only check AI level on server
	int32 CombatLevel = 1;

	virtual void SetupCombatInfo() override;
	//~ end Combat
};
