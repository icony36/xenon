// Copyright CoNY. All rights reserved.


#include "XeAssetManager.h"

#include "XeGameplayTags.h"

UXeAssetManager& UXeAssetManager::Get()
{
	check(GEngine);

	UXeAssetManager* XeAssetManager = Cast<UXeAssetManager>(GEngine->AssetManager);

	return *XeAssetManager;
}

void UXeAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Initialize all native gameplay tags.
	FXeGameplayTags::InitializeNativeGameplayTags();
}
