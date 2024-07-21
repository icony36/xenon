// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XENON_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)  // * BlueprintNativeEvent allow executes without casting, it automatically generates virtual native version that exist in C++ that can be overriden in C++
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	FVector GetSocketLocation(const FGameplayTag& SocketTag);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCombatLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	bool GetIsDead() const;
};
