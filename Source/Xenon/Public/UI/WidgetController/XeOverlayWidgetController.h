// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/XeWidgetController.h"
#include "XeOverlayWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXeOverlayWidgetController : public UXeWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnMaxHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnHealthRegenChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnMaxManaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnManaRegenChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnDamageChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnArmorChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnMovementSpeedChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnAttributeChanged OnEXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnCombatStateChanged OnCombatLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Combat|Attribute")
	FOnCombatStateChanged OnSkillPointChangedDelegate;

private:
	static float GetEXPPercent(const int32 NewEXP);
};
