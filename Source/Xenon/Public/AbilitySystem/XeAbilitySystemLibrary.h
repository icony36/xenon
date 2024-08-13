// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XeAbilitySystemLibrary.generated.h"

struct FEffectProperties;
class UCharacterInfo;
class UXeOverlayWidgetController;
/**
 * 
 */
UCLASS()
class XENON_API UXeAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//~ Helper Functions
	static void ShowDebugSphere(const UObject* WorldContextObject, const FVector& Center, const float Radius, const int32 Segments, const FLinearColor Color, const float LifeTime);
	//~ Helper Functions
	
	//~ Widget Controller
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AXeHUD*& OutXeHUD);
	
	UFUNCTION(BlueprintPure, Category="XeAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UXeOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	//~ end Widget Controller

	
	//~ Gameplay Mechanics
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveCombatActorsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& Center, const float Radius, const bool bShowDebug = false, const float ShowDebugTime = 1.f, const FLinearColor DebugColor = FLinearColor::Red);

	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|GameplayMechanics")
	static AActor* GetNearestCombatActor(const AActor* CenterActor, const TArray<AActor*>& ActorsToIgnore, const float Radius, const bool bShowDebug = false, const float ShowDebugTime = 1.f, const FLinearColor DebugColor = FLinearColor::Green); 
	//~ end Gameplay Mechanics
};
