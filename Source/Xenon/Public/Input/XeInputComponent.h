// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "XeInputTagConfig.h"
#include "XeInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXeInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// Template function for Player Controller to bind function on Action with Input Tag.
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UXeInputTagConfig* InputTagConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UXeInputComponent::BindAbilityActions(const UXeInputTagConfig* InputTagConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	checkf(InputTagConfig, TEXT("InputTagConfig is not set in BP_XePlayerController."));

	for (const FXeInputProperties& Props: InputTagConfig->AbilityInputProps)
	{
		if (Props.InputAction && Props.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				// Bind action to Enhanced Input Component with Input Tag as argument.
				BindAction(Props.InputAction, ETriggerEvent::Started, Object, PressedFunc, Props.InputTag);
			}

			if (ReleasedFunc)
			{
				// Bind action to Enhanced Input Component with Input Tag as argument.
				BindAction(Props.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Props.InputTag);
			}
			
			if (HeldFunc)
			{
				// Bind action to Enhanced Input Component with Input Tag as argument.
				BindAction(Props.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Props.InputTag);
			}
		}
	}
}
