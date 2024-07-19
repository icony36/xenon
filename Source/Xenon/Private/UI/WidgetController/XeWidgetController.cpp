// Copyright CoNY. All rights reserved.


#include "UI/WidgetController/XeWidgetController.h"

#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Player/XePlayerController.h"
#include "Player/XePlayerState.h"

void UXeWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	XePlayerController = CastChecked<AXePlayerController>(WidgetControllerParams.PlayerController);
	XePlayerState = CastChecked<AXePlayerState>(WidgetControllerParams.PlayerState);
	XeAbilitySystemComponent = CastChecked<UXeAbilitySystemComponent>(WidgetControllerParams.AbilitySystemComponent);
	XeAttributeSet = CastChecked<UXeAttributeSet>(WidgetControllerParams.AttributeSet);
}

void UXeWidgetController::BroadcastInitialValues()
{
	// Implement in child class.
}

void UXeWidgetController::BindCallbacksToDependencies()
{
	// Implement in child class.
}

