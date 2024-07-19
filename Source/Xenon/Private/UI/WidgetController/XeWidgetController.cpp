// Copyright CoNY. All rights reserved.


#include "UI/WidgetController/XeWidgetController.h"

#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Player/XePlayerController.h"
#include "Player/XePlayerState.h"

void UXeWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UXeWidgetController::BroadcastInitialValues()
{
	// Implement in child class.
}

void UXeWidgetController::BindCallbacksToDependencies()
{
	// Implement in child class.
}

AXePlayerController* UXeWidgetController::GetXePlayerController()
{
	if (XePlayerController == nullptr)
	{
		XePlayerController = Cast<AXePlayerController>(PlayerController);
	}

	return XePlayerController;
}

AXePlayerState* UXeWidgetController::GetXePlayerState()
{
	if (XePlayerState == nullptr)
	{
		XePlayerState = Cast<AXePlayerState>(PlayerState);
	}

	return XePlayerState;
}

UXeAbilitySystemComponent* UXeWidgetController::GetXeAbilitySystemComponent()
{
	if (XeAbilitySystemComponent == nullptr)
	{
		XeAbilitySystemComponent = Cast<UXeAbilitySystemComponent>(AbilitySystemComponent);
	}

	return XeAbilitySystemComponent;
}

UXeAttributeSet* UXeWidgetController::GetXeAttributeSet()
{
	if (XeAttributeSet == nullptr)
	{
		XeAttributeSet = Cast<UXeAttributeSet>(AttributeSet);
	}

	return XeAttributeSet;
}
