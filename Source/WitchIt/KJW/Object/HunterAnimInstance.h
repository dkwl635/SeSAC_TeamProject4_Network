// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HunterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UHunterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	virtual void NativeInitializeAnimation () override;
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

public:
	void PlayshootPotato ();

protected:
	
	//이동 방향
	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = MyAnimSettings , meta = (AllowPrivateAccess = "true") )
	float Direction;
	//이동속도
	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = MyAnimSettings , meta = (AllowPrivateAccess = "true") )
	float Speed;
	//
	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = MyAnimSettings , meta = (AllowPrivateAccess = "true") )
	FVector Velocity;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = MyAnimSettings , meta = (AllowPrivateAccess = "true") )
	float GroundSpeed;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = MyAnimSettings , meta = (AllowPrivateAccess = "true") )
	bool bShouldMove = false;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = MyAnimSettings , meta = (AllowPrivateAccess = "true") )
	bool IsFalling = false;

	UPROPERTY ( EditAnywhere , BlueprintReadOnly , Category = MyAnimSettings , meta = (AllowPrivateAccess = "true") )
	bool IsFirstView = false;

	UPROPERTY ()
	class AHunter* Hunter;

	//감자 던지기 몽타주
	UPROPERTY ( EditDefaultsOnly , Category = MyAnimSettings )
	class UAnimMontage* ShootPotatoMontage;

	//허리 각도 
	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = MyAnimSettings )
	float PitchAngle;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComp;

private:
	//감자 던지기 애니메이션 노티파이 이벤트
	UFUNCTION ()
	void AnimNotify_OnShootPotota ();

	UFUNCTION ()
	void AnimNotify_OnShootEndPotota ();


};
