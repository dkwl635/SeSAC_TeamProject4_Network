// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chicken.generated.h"

UCLASS()
class WITCHIT_API AChicken : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChicken();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	//서버에서만 호출하게
	void FindWitchs ();
	void RecoverUpright ( float DeltaTime );
	
	UFUNCTION ( NetMulticast , Reliable )
	void MulticastRPC_ShowFindEffect (bool bShow);

	UFUNCTION ()
	void OnHitComponent ( UPrimitiveComponent* HitComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector  NormalImpulse , const FHitResult& Hit );



private:

	UPROPERTY ( EditDefaultsOnly )
	class USphereComponent* SphereComp;
	UPROPERTY ( EditDefaultsOnly )
	USkeletalMeshComponent* MeshComp;


	UPROPERTY ( EditDefaultsOnly , Category = Chicken )
	class UNiagaraComponent* FindEffectComp;

	UPROPERTY ( EditDefaultsOnly , Category = Chicken )
	class UNiagaraComponent* AuraEffectComp;
private:
	UPROPERTY ()
	class AHASGameState* GS;

	UPROPERTY( EditDefaultsOnly , Category = Chicken)
	float FindDistance = 250.f;


	FTimerHandle FindWitchTimerHandle;
};
