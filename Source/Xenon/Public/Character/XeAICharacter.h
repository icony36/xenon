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
	
protected:
	virtual void BeginPlay() override;
	
	
	//~ Combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI Character Class")
	int32 CombatLevel = 1;

	virtual void SetupCombatInfo() override;
	//~ end Combat
};
