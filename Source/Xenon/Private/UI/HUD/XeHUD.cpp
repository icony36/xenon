// Copyright CoNY. All rights reserved.


#include "UI/HUD/XeHUD.h"

#include "UI/Widget/XeUserWidget.h"
#include "UI/WidgetController/XeOverlayWidgetController.h"

void AXeHUD::InitializeOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is not set in BP_HUD."));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is not set in BP_HUD."));

	// Create Overlay Widget Controller.
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UXeOverlayWidgetController* XeOverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// Create Overlay Widget.
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

	// Cast to XeUserWidget to add Widget Controller.
	OverlayWidget = Cast<UXeUserWidget>(Widget);

	// Add OverlayWidgetController and trigger WidgetControllerSet event in XeUserWidget.
	OverlayWidget->SetWidgetController(XeOverlayWidgetController); 

	// Broadcast initial values of attributes.
	XeOverlayWidgetController->BroadcastInitialValues();

	// Add Overlay Widget to viewport.
	OverlayWidget->AddToViewport(); 
}

UXeOverlayWidgetController* AXeHUD::GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams)
{
	if (OverlayWidgetController == nullptr)
	{
		// Create Widget Controller.
		OverlayWidgetController = NewObject<UXeOverlayWidgetController>(this, OverlayWidgetControllerClass);

		// Set Widget Controller params (Player State, Player Controller, Ability System Component, Attribute Set)
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);

		// Bind delegates for changes.
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return  OverlayWidgetController;
}

void AXeHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unbind delegate callbacks.
	if (OverlayWidgetController)
	{
		OverlayWidgetController->UnbindCallbacksToDependencies();
	}
	
	Super::EndPlay(EndPlayReason);
}

