// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "XePlayerController.generated.h"


class UXeAbilitySystemComponent;
struct FGameplayTag;
class UXeInputTagConfig;
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

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UXeInputTagConfig> InputTagConfig;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	void MoveCompleted(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);
	//~ end Input


	//~ Ability System
	TObjectPtr<UXeAbilitySystemComponent> XeAbilitySystemComponent;

	UXeAbilitySystemComponent* GetAbilitySystemComponent();
	//~ end Ability System
};
