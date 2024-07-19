// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/XeCharacter.h"
#include "XePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class XENON_API AXePlayerCharacter : public AXeCharacter
{
	GENERATED_BODY()

public:
	AXePlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual auto OnRep_PlayerState() -> void override;

	//~ Combat Interface
	virtual int32 GetCombatLevel_Implementation() override;
	//~ end Combat Interface

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	//~ Combat
	virtual void SetupCombatInfo() override;

	virtual void SetupOverheadWidget() override;
	//~ Combat
};
