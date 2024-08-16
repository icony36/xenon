// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "XeAnimNotifyState.generated.h"

class AXePlayerCharacter;
/**
 * 
 */
UCLASS()
class XENON_API UXeAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="XeAnimNotifyState")
	TObjectPtr<AXePlayerCharacter> PlayerCharacter;
};
