// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XeAbilitySystemLibrary.generated.h"

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
	//~ Widget Controller
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AXeHUD*& OutXeHUD);
	
	UFUNCTION(BlueprintPure, Category="XeAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UXeOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	//~ end Widget Controller

	
	// Character Class Info
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|CharacterDefaults")
	static UCharacterInfo* GetCharacterInfo(const UObject* WorldContextObject);

	static int32 GetEXPReward(const UObject* WorldContextObject, const FGameplayTag& CharacterTag, int32 CharacterLevel);
	//~ end Character Class Info

	
	//~ Gameplay Mechanics
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|GameplayMechanics")
	static void SendEXP(const UObject* WorldContextObject, AActor* Recipient,  float InEXP);
	
	UFUNCTION(BlueprintCallable, Category="XeAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereOrigin, float Radius, bool bShowDebug = false);
	//~ end Gameplay Mechanics
	
};
