// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WitchAnim.generated.h"

/**
 * 
 */



UCLASS()
class WITCHIT_API UWitchAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UWitchAnim();
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps ( TArray<class FLifetimeProperty>& OutLifetimeProps ) const override;

public:
	

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	float FlyUpSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrouchDireciton = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector velocity; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector forwardVector; 

	UPROPERTY(EditAnywhere, Replicated , BlueprintReadWrite)
	FVector upVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWitch* player = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCrouched = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsJumping = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFlying = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWitch* witch = nullptr; 

	UPROPERTY()
	class UCharacterMovementComponent* MovementComp;	

};
