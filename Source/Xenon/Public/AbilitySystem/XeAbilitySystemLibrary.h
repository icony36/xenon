// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
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
	//~ Debug
	static void ShowDebugSphere(const UObject* WorldContextObject, const FVector& Center, const float Radius, const int32 Segments, const FLinearColor Color, const float LifeTime);
	//~ end Debug

	
	//~ Widget Controller
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AXeHUD*& OutXeHUD);
	
	UFUNCTION(BlueprintPure, Category="XeAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UXeOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	//~ end Widget Controller

	
	//~ Gameplay Mechanics
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|GameplayMechanics")
	static bool GetIsChanceSuccess(const float Chance);
	
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveCombatActorsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& Center, const float Radius, const bool bShowDebug = false, const float ShowDebugTime = 1.f, const FLinearColor DebugColor = FLinearColor::Red);

	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|GameplayMechanics")
	static AActor* GetNearestCombatActor(const AActor* CenterActor, const TArray<AActor*>& ActorsToIgnore, const float Radius, const bool bShowDebug = false, const float ShowDebugTime = 1.f, const FLinearColor DebugColor = FLinearColor::Green); 
	//~ end Gameplay Mechanics


	//~ Effect Context
	UFUNCTION(BlueprintPure, Category = "XeAbilitySystemLibrary|GameplayEffectContext")
	static FGameplayTag GetHitReactTag(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "XeAbilitySystemLibrary|GameplayEffectContext")
	static FGameplayTag GetCriticalAbilityTag(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "XeAbilitySystemLibrary|GameplayEffectContext")
	static FGameplayTag GetBlockAbilityTag(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "XeAbilitySystemLibrary|GameplayEffectContext")
	static void SetHitReactTag(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InHitReactTag);

	UFUNCTION(BlueprintCallable, Category = "XeAbilitySystemLibrary|GameplayEffectContext")
	static void SetCriticalAbilityTag(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InCriticalAbilityTag);
	
	UFUNCTION(BlueprintCallable, Category = "XeAbilitySystemLibrary|GameplayEffectContext")
	static void SetBlockAbilityTag(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InBlockAbilityTag);
	//~ end Effect Context
};
