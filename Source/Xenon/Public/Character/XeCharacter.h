// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/XeWidgetController.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "XeCharacter.generated.h"

class UWidgetComponent;
class UGameplayAbility;
class UGameplayEffect;
class UXeAttributeSet;

UCLASS(Abstract)
class XENON_API AXeCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AXeCharacter();


	//~ IAbilitySystem Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
	//~ end IAbilitySystem Interface

	
	//~ Combat Interface
	virtual AActor* GetAvatar_Implementation() override;
	virtual FVector GetSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	virtual FGameplayTag GetCharacterTag_Implementation() override;
	virtual bool GetIsDead_Implementation() const override;
	//~ end Combat Interface
	
protected:
	//~ Combat Interface
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Combat")
	FGameplayTag CharacterTag;
	
	bool IsDead = false;
	//~ end Combat Interface
	
	
	//~ Ability System
	UPROPERTY()
	TObjectPtr<UXeAbilitySystemComponent> XeAbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UXeAttributeSet> XeAttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Combat|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Combat|Attributes")
	TSubclassOf<UGameplayEffect> DefaultEssentialAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Combat|Attributes")
	TSubclassOf<UGameplayEffect> DefaultBasicAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultAdvancedAttributes;
	
	virtual void InitializeDefaultAttributes() const;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	
	void AddStartupAbilities() const;
	//~ end Ability System

	
	//~ Combat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadWidget;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnMaxHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnManaChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnMaxManaChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnCombatStateChanged OnCombatLevelChangedDelegate;

	virtual void SetupCombatInfo();

	virtual void SetupOverheadWidget();

	virtual void BindCallbacksToDependencies();
	//~ end Combat

private:
	//~ Ability System
	UPROPERTY(EditAnywhere, Category="Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupActiveAbilities;
	
	UPROPERTY(EditAnywhere, Category="Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	//~ end Ability System
};
