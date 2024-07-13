// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "XePlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class XENON_API AXePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AXePlayerController();
	
protected:
	virtual void BeginPlay() override;

	//~ Input
	virtual void SetupInputComponent() override;
	//~ end Input
	
private:

	//~ Input
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	//~ end Input
	
};
