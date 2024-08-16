// Copyright CoNY. All rights reserved.


#include "AnimNotify/XeAnimNotifyState.h"

#include "Character/XePlayerCharacter.h"

void UXeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (AXePlayerCharacter* Character = Cast<AXePlayerCharacter>(Owner))
		{
			PlayerCharacter = Character;
		}
	}
}
