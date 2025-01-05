// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XENON_API IPoolableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Poolable")
	void OnSpawnFromPool();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Poolable")
	void OnReturnToPool();
};
