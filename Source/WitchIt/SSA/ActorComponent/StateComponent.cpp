// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/ActorComponent/StateComponent.h"
#include "Components/ActorComponent.h"
#include "WitchIt.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	APawn* OhterPlayer = Cast<APawn>(GetOwner());
}

// Called every frame
void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStateComponent::TakeDamage ( float DamageAmount )
{
	ServerRPC_TakeDamage( DamageAmount );
}

void UStateComponent::ServerRPC_TakeDamage_Implementation (float DamageAmount )
{
	SetCurHP ( CurHP - DamageAmount );

	APawn* OhterPlayer = Cast<APawn> ( GetOwner () );
}


void UStateComponent::SetDamageAmount ( float DamageAmount )
{
	Damage += DamageAmount;
}

void UStateComponent::ConsumeMana (float Amount)
{
	if(CurMana <= 0.0f) return;

	CurMana = FMath::Clamp(CurMana - Amount, 0.0f, MaxMana);

	if (CurMana <= 0.0f && !bManaEmpty)
	{
		bManaEmpty = true;
	}

	StateUpdateDelegate.Broadcast ();
}

void UStateComponent::RegenMana ( float Amount )
{
	CurMana = FMath::Clamp(CurMana + Amount, 0.0f, MaxMana);

	if (CurMana > 0.0f && bManaEmpty)
	{
		bManaEmpty = false; 
	}

	StateUpdateDelegate.Broadcast ();
}

void UStateComponent::RecoverStamina ( float Amount )
{
}

void UStateComponent::ConsumeStamina ( float Amount )
{
}

void UStateComponent::ApplyHPRatio ( float Ratio )
{
	MaxHP = FMath::Max   ( MaxHP , 1.0f ); // 0 방지
	CurHP = FMath::Clamp ( MaxHP * Ratio , 0.0f , MaxHP );

	StateUpdateDelegate.Broadcast();
}

void UStateComponent::SetHPFromRatio ( float NewMaxHP , float Ratio )
{
	MaxHP = NewMaxHP;
	CurHP = FMath::Clamp ( MaxHP * Ratio , 0.0f , MaxHP );
	
	StateUpdateDelegate.Broadcast ();
}

void UStateComponent::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME ( UStateComponent, CurHP   );
	DOREPLIFETIME ( UStateComponent, CurMana );
}

