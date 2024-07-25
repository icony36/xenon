// Copyright CoNY. All rights reserved.


#include "Character/XeCharacter.h"

#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

AXeCharacter::AXeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup status widget above head.
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>("OverheadWidget");
	OverheadWidget->SetupAttachment(GetRootComponent());
}

UAbilitySystemComponent* AXeCharacter::GetAbilitySystemComponent() const
{
	return XeAbilitySystemComponent;
}

UAttributeSet* AXeCharacter::GetAttributeSet() const
{
	return XeAttributeSet;
}

AActor* AXeCharacter::GetAvatar_Implementation() 
{
	return this;
}

FVector AXeCharacter::GetSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	const FXeGameplayTags& GameplayTags = FXeGameplayTags::Get();
	if (SocketTag.MatchesTagExact(GameplayTags.Socket_WeaponTip))
	{
		return GetMesh()->GetSocketLocation("WeaponTipSocket");
	}

	if (SocketTag.MatchesTagExact(GameplayTags.Socket_LeftHand))
	{
		return GetMesh()->GetSocketLocation("LeftHandSocket");
	}

	if (SocketTag.MatchesTagExact(GameplayTags.Socket_RightHand))
	{
		return GetMesh()->GetSocketLocation("RightHandSocket");
	}

	if (SocketTag.MatchesTagExact(GameplayTags.Socket_Tail))
	{
		return GetMesh()->GetSocketLocation("TailSocket");
	}

	return FVector();
}

FGameplayTag AXeCharacter::GetCharacterTag_Implementation()
{
	return CharacterTag;
}

bool AXeCharacter::GetIsDead_Implementation() const
{
	return bIsDead;
}

void AXeCharacter::Die_Implementation(float RespawnTime)
{
	MulticastHandleDeath();

	if (RespawnTime > 0)
	{
		// Start respawn cooldown.
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &ThisClass::RespawnTimerFinished, RespawnTime);
	}
	else if (RespawnTime == 0) // * SetTimer with 0 time will clear the existing timer
	{
		RespawnTimerFinished();
	}
}

void AXeCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindCallbacksFromDependencies();
	
	Super::EndPlay(EndPlayReason);
}

void AXeCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultBasicAttributes, 1.f);
	ApplyEffectToSelf(DefaultEssentialAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f); // * apply Vital Attributes last as they depend on Essential Attributes
}

void AXeCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	
	check(IsValid(ASC));
	check(GameplayEffectClass);
	
	// Apply effect.
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),ASC);
}

void AXeCharacter::AddStartupAbilities() const
{
	if (!HasAuthority()) return;
	
	XeAbilitySystemComponent->AddCharacterActiveAbilities(StartupActiveAbilities);
	XeAbilitySystemComponent->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AXeCharacter::InitializeCharacter()
{
	// Implement in child class.
}

void AXeCharacter::SetupOverheadWidget()
{
	// Implement in child class.
}

void AXeCharacter::BindCallbacksToDependencies()
{
	// Implement in child class.
}

void AXeCharacter::UnbindCallbacksFromDependencies()
{
	// Implement in child class.
}

void AXeCharacter::BroadcastInitialValues()
{
	// Implement in child class.
}

void AXeCharacter::MulticastHandleDeath_Implementation()
{
	// Set bIsDead to true (calling in Multicast will apply to both client and server without making it replicated).
	bIsDead = true;
	
	OnDeathDelegate.Broadcast(this);

	PlayDeathEffects();
}

void AXeCharacter::PlayDeathEffects()
{
	// Set ragdoll effect.
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// Disable collision.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AXeCharacter::RespawnTimerFinished()
{
	// Implement in child class.
}

