// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/XeCharacter.h"
#include "Interface/PlayerInterface.h"
#include "Player/XePlayerState.h"
#include "XePlayerCharacter.generated.h"

class AXeGameMode;
class UNiagaraComponent;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class XENON_API AXePlayerCharacter : public AXeCharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AXePlayerCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called in server only.
	virtual void PossessedBy(AController* NewController) override;

	// Called in client only.
	virtual void OnRep_PlayerState() override;

	
	//~ Combat Interface
	virtual int32 GetCombatLevel_Implementation() override;

	virtual void AddCriticalData_Implementation(const FGameplayTag& AbilityTag, const float CriticalChance, const float CriticalRate) override;
	virtual void AddBlockData_Implementation(const FGameplayTag& AbilityTag, const float BlockChance, const float BlockRate, const bool bIsBlockRatePercentage) override;
	virtual void UpdateCriticalData_Implementation(const FGameplayTag& AbilityTag, const float CriticalChance, const float CriticalRate) override;
	virtual void UpdateBlockData_Implementation(const FGameplayTag& AbilityTag, const float BlockChance, const float BlockRate, const bool bIsBlockRatePercentage) override;
	virtual void RemoveCriticalData_Implementation(const FGameplayTag& AbilityTag) override;
	virtual void RemoveBlockData_Implementation(const FGameplayTag& AbilityTag) override;
	virtual FGameplayTag GetChosenCriticalData_Implementation(FCriticalData& OutData) override;
	virtual FGameplayTag GetChosenBlockData_Implementation(FBlockData& OutData) override;
	//~ end Combat Interface


	//~ Player Interface
	virtual int32 FindCombatLevelWithEXP_Implementation(int32 InEXP) override;

	virtual int32 GetEXP_Implementation() override;

	virtual int32 GetSkillPoint_Implementation() override;

	virtual FLevelUpProperties GetLevelUpProperties_Implementation(int32 Level) override;

	virtual void AddToCombatLevel_Implementation(int32 InCombatLevel) override;

	virtual void AddToEXP_Implementation(int32 InEXP) override;

	virtual void AddToSkillPoint_Implementation(int32 InSkillPoint) override;

	virtual void LevelUp_Implementation() override;
	//~ end Player Interface


protected:
	UPROPERTY()
	TObjectPtr<AXePlayerState> XePlayerState;

	
	//~ Xe Character
	virtual void InitializeCharacter() override;

	virtual void BindCallbacksToDependencies() override;

	virtual void UnbindCallbacksFromDependencies() override;

	virtual void BroadcastInitialValues() override;

	virtual void SetupOverheadWidget() override;
	//~ end Xe Character

	
	//~ Xe Player Character
	void SetupHUD() const; 
	//~ end Xe Player Character


	//~ Level Up
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayLevelUpEffects() const;
	//~ end Level Up


	//~ Death
	virtual void Die_Implementation(float RespawnTime) override;
	
	virtual void RespawnTimerFinished() override;
	//~ end Death
	
private:
	//~ Camera
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	//~ end Camera

	void HandleFirstSpawn() const;
	void HandleRespawn() const;
};
