// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "XePlayerState.generated.h"

class UAttributeSet;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, int32 /*StateValue*/);

/**
 * 
 */
UCLASS()
class XENON_API AXePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	//~ IAbilitySystem Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	//~ end IAbilitySystem Interface
	
	
	//~ Combat
	FOnPlayerStateChanged OnCombatLevelChangedDelegate; 
	FOnPlayerStateChanged OnExperienceChangedDelegate;
	FOnPlayerStateChanged OnSkillPointChangedDelegate;
	
	FORCEINLINE int32 GetCombatLevel() const { return CombatLevel; }
	FORCEINLINE int32 GetExperience() const { return Experience; }
	FORCEINLINE int32 GetSkillPoint() const { return SkillPoint; }
	
	void SetCombatLevel(int32 InCombatLevel);
	void SetExperience(int32 InExperience);
	void SetSkillPoint(int32 InSkillPoint);
	
	void AddToCombatLevel(int32 Value);
	void AddToExperience(int32 Value);
	void AddToSkillPoint(int32 Value);
	//~ end Combat

protected:
	//~ Ability System
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	//~ end Ability System
	
private:
	//~ Combat
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_CombatLevel)
	int32 CombatLevel = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Experience)
	int32 Experience = 0;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SkillPoint)
	int32 SkillPoint = 0;
	
	UFUNCTION()
	void OnRep_CombatLevel(int32 OldValue) const;
	
	UFUNCTION()
	void OnRep_Experience(int32 OldValue) const;
	
	UFUNCTION()
	void OnRep_SkillPoint(int32 OldValue) const;
	//~ end Combat
};
