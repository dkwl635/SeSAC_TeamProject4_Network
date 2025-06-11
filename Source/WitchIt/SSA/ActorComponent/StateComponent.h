// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

DECLARE_MULTICAST_DELEGATE ( FStateUpdateDelegate );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WITCHIT_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UStateComponent* SetCurHP      ( float InCurHP      ) { this->CurHP      = InCurHP;       StateUpdateDelegate.Broadcast(); return this; }
	UStateComponent* SetMaxHP      ( float InMaxHp      ) { this->MaxHP      = InMaxHp;       StateUpdateDelegate.Broadcast(); return this; }
	UStateComponent* SetCurMana    ( float InCurMana    ) { this->CurMana    = InCurMana;     StateUpdateDelegate.Broadcast(); return this; }
	UStateComponent* SetMaxMana    ( float InMaxMana    ) { this->MaxMana    = InMaxMana;     StateUpdateDelegate.Broadcast(); return this; }
	UStateComponent* SetDamage     ( float InDamage     ) { this->Damage     = InDamage;      StateUpdateDelegate.Broadcast(); return this; }
	UStateComponent* SetCurStamena ( float InCurStamena ) { this->CurStamina = InCurStamena;  StateUpdateDelegate.Broadcast(); return this; }
	UStateComponent* SetMaxStamena ( float InMaxStamena ) { this->MaxStamina = InMaxStamena;  StateUpdateDelegate.Broadcast(); return this; }


public:
	float GetCurHP   () const { return CurHP;   }
	float GetMaxHP   () const { return MaxHP;   }
	float GetCurMana () const { return CurMana; }
	float GetMaxMana () const { return MaxMana; }
	float GetHPRatio () const { return (MaxHP > 0.0f) ? (CurHP / MaxHP) : 0.0f; }
 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StateComponent)
	float MaxHP = 100.0f;

	//UPROPERTY(ReplicatedUsing = OnRep_CurHP, BlueprintReadOnly, Category = StateComponent)
	
	/*UPROPERTY(ReplicatedUsing=OnRep_CurHP, BlueprintReadOnly, Category = StateComponent)*/
	UPROPERTY(Replicated, VisibleAnywhere, Category = StateComponent)
	float CurHP = MaxHP;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StateComponent)
	float MaxMana = 100.0f; 

	UPROPERTY(Replicated , BlueprintReadOnly , Category = StateComponent)
	float CurMana = MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StateComponent)
	float MaxStamina = 100.0f;

	UPROPERTY(Replicated , BlueprintReadOnly , Category = StateComponent)
	float CurStamina = MaxStamina;

	UPROPERTY(Replicated , BlueprintReadOnly , Category = StateComponent)
	float Damage = 0.0f;

public:

	void TakeDamage ( float DamageAmount );

	void SetDamageAmount ( float DamageAmount ); 

	void ConsumeMana( float Amount );

	void RegenMana( float Amount );

	void RecoverStamina( float Amount );

	void ConsumeStamina( float Amount );

	bool IsManaEmpty() const { return bManaEmpty; }

	void ApplyHPRatio ( float Ratio );

	void SetHPFromRatio ( float NewMaxHP , float Ratio );


public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakeDamage( float DamageAmount );


	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	bool bManaEmpty = false;
	bool bManaFull  = false;


	FStateUpdateDelegate StateUpdateDelegate;

public:

};
