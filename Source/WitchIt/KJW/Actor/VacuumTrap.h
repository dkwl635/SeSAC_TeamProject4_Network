// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VacuumTrap.generated.h"

UCLASS()
class WITCHIT_API AVacuumTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVacuumTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	bool bTest = false;

private:
	UFUNCTION ()
	void VacuumEffect ();
	UFUNCTION ()
	void VacuumCharacterEffect();

	UFUNCTION ()
	void OnHitComponent ( UPrimitiveComponent* HitComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector  NormalImpulse , const FHitResult& Hit );

	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ShowEffect ();

private:
	UPROPERTY ( EditDefaultsOnly )
	UStaticMeshComponent* MeshComp;

	UPROPERTY ( EditDefaultsOnly , Category = VacuumTrap )
	class UNiagaraComponent* VacuumEffectComp;

	UPROPERTY ( EditDefaultsOnly , Category = Chicken )
	float VacuumDistance = 250.f;

	//3초 동안 0.5초 마다 땡기기
	FTimerHandle VacuumTimerHandle;
	//3초 동안 0.02초 마다 땡기기 마녀 전용
	FTimerHandle VacuumTimerCharacterHandle;

	//GameState
	UPROPERTY()
	class AHASGameState* GS;

	bool IsHit = false;

	TArray<class AItemBase*> FindItems;
	TArray<class ABaseCharacter*> FindWitchs;
};
