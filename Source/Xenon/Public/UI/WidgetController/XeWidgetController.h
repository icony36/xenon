// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "XeWidgetController.generated.h"


class UXeAttributeSet;
class UXeAbilitySystemComponent;
class AXePlayerState;
class AXePlayerController;
class APlayerController;
class UAttributeSet;
class UAbilitySystemComponent;
class APlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCombatStateChanged, int32, NewValue);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class XENON_API UXeWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams);
	
	virtual void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	// TODO: Get Ability
	// void BroadcastAbilityInfo();

protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AXePlayerController> XePlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AXePlayerState> XePlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UXeAbilitySystemComponent> XeAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UXeAttributeSet> XeAttributeSet;
};
