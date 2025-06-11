// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Potato.generated.h"

UCLASS()
class WITCHIT_API APotato : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotato();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetPower (float NewPower);
public:
	UPROPERTY(EditAnywhere)
	float MoveSPD = 100.0f;

	UPROPERTY ( EditAnywhere )
	float Damage = 10.0f;

	UPROPERTY ( EditAnywhere )
	float Power = 1.0f;

	UPROPERTY ( VisibleAnywhere )
	class ABaseCharacter* OwnerHunter;
private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;


private:
	UFUNCTION()
	void OnHitComponent ( UPrimitiveComponent* HitComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector  NormalImpulse , const FHitResult& Hit );


	
};
