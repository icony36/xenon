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
	
	
	//~ Xe AI Character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat|AI") // * this is not replicated as we only check AI level on server
	int32 CombatLevel = 1;
	//~ end Xe AI Character

	
	//~ Xe Character
	virtual void InitializeCharacter() override;
	//~ end Xe Character
};
