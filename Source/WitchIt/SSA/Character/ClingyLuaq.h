// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClingyLuaq.generated.h"

UCLASS()
class WITCHIT_API AClingyLuaq : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClingyLuaq();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class USkeletalMeshComponent* MeshComp; 

public:

	UFUNCTION()
	void OnLuaqOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
					   bool bFromSweep, const FHitResult& SweepResult);

	void AttachToHunter(AActor* OtherActor );

	void Die();

private:
	UPROPERTY ( EditDefaultsOnly , Category = Effect )
	class UNiagaraComponent* AuraEffectComp;

public:
	FTimerHandle RemoveTimerHandle;

	class AHunter* Hunter;

	bool IsHit = false;
};
