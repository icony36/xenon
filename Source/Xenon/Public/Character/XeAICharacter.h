// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Character/XeCharacter.h"
#include "XeAICharacter.generated.h"

class UAttributeSet;
/**
 * 
 */
UCLASS()
class XENON_API AXeAICharacter : public AXeCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXeAICharacter();

	//~ IAbilitySystem Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	//~ end IAbilitySystem Interface
	
protected:
	//~ Ability System
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	//~ end Ability System
	
	//~ Combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI Character Class")
	int32 CombatLevel = 1;
	//~ end Combat
};
