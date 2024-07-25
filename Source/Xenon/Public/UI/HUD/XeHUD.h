// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XeHUD.generated.h"

struct FWidgetControllerParams;
class UXeOverlayWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UXeUserWidget;
/**
 * 
 */
UCLASS()
class XENON_API AXeHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitializeOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	// Get OverlayWidgetController, create one if it is null.
	UXeOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY()
	TObjectPtr<UXeUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere, Category="Overlay")
	TSubclassOf<UXeUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UXeOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere, Category="Overlay")
	TSubclassOf<UXeOverlayWidgetController> OverlayWidgetControllerClass;
	
};
