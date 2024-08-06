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
	virtual void Die_Implementation(float RespawnTime) override;
	//~ end Combat Interface
	
	
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
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
	TSubclassOf<UGameplayEffect> DefaultCombatAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Combat|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAdvancedAttributes;
	
	virtual void InitializeDefaultAttributes() const;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const;
	
	void AddStartupAbilities() const;
	//~ end Ability System

	
	//~ Combat
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
	
	FDelegateHandle OnHealthChangedDelegateHandle;
	FDelegateHandle OnMaxHealthChangedDelegateHandle;
	FDelegateHandle OnManaChangedDelegateHandle;
	FDelegateHandle OnMaxManaChangedDelegateHandle;
	FDelegateHandle OnCombatLevelChangedDelegateHandle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Combat")
	FGameplayTag CharacterTag;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadWidget;
	
	virtual void InitializeCharacter();

	virtual void BindCallbacksToDependencies();

	virtual void UnbindCallbacksFromDependencies();

	virtual void BroadcastInitialValues(); 

	virtual void SetupOverheadWidget();
	//~ end Combat

	
	// Death
	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false;

	FTimerHandle RespawnTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* DeathSound;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathDelegate;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(); // RespawnTime < 0 -> No respawn.

	virtual void PlayDeathEffects();

	virtual void RespawnTimerFinished();
	//~ end Death

private:
	//~ Ability System
	UPROPERTY(EditAnywhere, Category="Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupActiveAbilities;
	
	UPROPERTY(EditAnywhere, Category="Combat|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	//~ end Ability System
};
