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

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
};
