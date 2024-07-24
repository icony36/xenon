// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/XeCharacter.h"
#include "Interface/PlayerInterface.h"
#include "XePlayerCharacter.generated.h"

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

	// Called in server only.
	virtual void PossessedBy(AController* NewController) override;

	// Called in client only.
	virtual auto OnRep_PlayerState() -> void override;

	//~ Combat Interface
	virtual int32 GetCombatLevel_Implementation() override;
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
	virtual void BeginPlay() override;


	//~ Combat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	virtual void SetupCombatInfo() override;

	virtual void SetupOverheadWidget() override;

	virtual void BindCallbacksToDependencies() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayLevelUpEffects() const;
	//~ Combat
	
private:
	UPROPERTY()
	AXePlayerController* XePlayerController = nullptr;

	UPROPERTY()
	AXePlayerState* XePlayerState = nullptr;
	
	//~ Camera
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	//~ end Camera
	
};
