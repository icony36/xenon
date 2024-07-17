// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "XeCharacter.generated.h"

class UAttributeSet;

UCLASS(Abstract)
class XENON_API AXeCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXeCharacter();

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
	virtual void SetupCombatInfo();
	//~ end Combat
};
