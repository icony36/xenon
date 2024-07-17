// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "XeAssetManager.generated.h"

/**
 * Singleton - responsible for loading and unloading PrimaryAssets.
 */
UCLASS()
class XENON_API UXeAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UXeAssetManager& Get();

protected:
	// Called when asset manager start initial loading.
	virtual void StartInitialLoading() override;
};
