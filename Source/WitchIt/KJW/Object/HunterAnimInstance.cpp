// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Object/HunterAnimInstance.h"
#include "KJW/Actor/Hunter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UHunterAnimInstance::NativeInitializeAnimation ()
{
	Super::NativeInitializeAnimation ();

	Hunter = Cast<AHunter> ( TryGetPawnOwner () );
	if(Hunter)
	MovementComp = Hunter->GetCharacterMovement ();

}

void UHunterAnimInstance::NativeUpdateAnimation ( float DeltaSeconds )
{
	Super::NativeUpdateAnimation ( DeltaSeconds );

	if (!Hunter|| !MovementComp) return;
	
	
	//현제 이동속도, 방향, 각도 확인
	Speed = FVector::DotProduct ( Hunter->GetVelocity () , Hunter->GetActorForwardVector () );
	Direction = FVector::DotProduct ( Hunter->GetVelocity () , Hunter->GetActorRightVector () );
	PitchAngle = -Hunter->GetBaseAimRotation ().GetNormalized ().Pitch;
	PitchAngle = FMath::Clamp ( PitchAngle , -30.0f , 30.0f );

	//Movement 변수 저장 
	IsFalling = MovementComp->IsFalling ();
	Velocity = MovementComp->Velocity;
	GroundSpeed = UKismetMathLibrary::VSizeXY ( Velocity );

	bShouldMove = (!MovementComp->GetCurrentAcceleration ().IsNearlyZero () && GroundSpeed > 3.0f);

	IsFirstView = Hunter->bFirstPersonView;
}

void UHunterAnimInstance::PlayshootPotato ()
{
	if (!Hunter || !ShootPotatoMontage) return;
	//현제 애니몽타주 실행 중이면 안함
	//if (Montage_IsPlaying ( ShootPotatoMontage )) return;
	Montage_Play ( ShootPotatoMontage );

}

void UHunterAnimInstance::AnimNotify_OnShootPotota ()
{
	if (!Hunter)return;
	//Hunter->SpawnPotato ();
	//Hunter->ShowRightPotato ( false );
}

void UHunterAnimInstance::AnimNotify_OnShootEndPotota ()
{
	if (!Hunter)return;
	Hunter->ShowRightPotato ( true );

}
