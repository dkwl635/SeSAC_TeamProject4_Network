// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Character/WitchAnim.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Witch.h"

UWitchAnim::UWitchAnim ()
{

}

void UWitchAnim::NativeBeginPlay ()
{
	Super::NativeBeginPlay ();

	witch = Cast<AWitch> ( TryGetPawnOwner () );

	if (witch)
	MovementComp = witch->GetCharacterMovement ();
	
}

void UWitchAnim::NativeUpdateAnimation ( float DeltaSeconds )
{
	Super::NativeUpdateAnimation ( DeltaSeconds );

	if (witch == nullptr || MovementComp == nullptr) return;

	velocity = witch->GetVelocity();
	forwardVector = witch->GetActorForwardVector();

	upVector = witch->GetActorUpVector ();

	Speed = FVector::DotProduct( forwardVector , velocity );

	FlyUpSpeed = FVector::DotProduct ( upVector , velocity );

	FVector rightVector = witch->GetActorRightVector();
	Direction = FVector::DotProduct(rightVector, velocity );

	FRotator BaseRotator = witch->GetActorRotation();

	//CrouchDireciton = CalculateDirection (velocity, BaseRotator );

	bIsCrouched = witch->bIsCrouched;

	bIsJumping = MovementComp->IsFalling ();

	bIsFlying = witch->bIsFlying;
}

void UWitchAnim::GetLifetimeReplicatedProps ( TArray<class FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( UWitchAnim , FlyUpSpeed );
	DOREPLIFETIME ( UWitchAnim , upVector   );
}
