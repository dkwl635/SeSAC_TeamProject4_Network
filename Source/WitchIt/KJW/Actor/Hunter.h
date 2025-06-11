// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/Actor/BaseCharacter.h"
#include "InputActionValue.h"
#include "Hunter.generated.h"

UCLASS()
class WITCHIT_API AHunter : public ABaseCharacter
{
	GENERATED_BODY()


public:
	friend class UHunterAnimInstance;

public:
	// Sets default values for this pawn's properties
	AHunter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay () override;
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps )  const override;


public:
	// Called every frame
	virtual void Tick ( float DeltaTime ) override;

public:
	void SpawnPotato ( float Power );
	void ShowRightPotato (bool bShow);
	void HitLuaq (bool IsStart);
	FVector GetTraceFromView ();
	FVector GetRightHandLocation ();
private:
	
	virtual void SetupPlayerInputComponent ( class UInputComponent* PlayerInputComponent ) override;

	void Move ( const FInputActionValue& InputValue );
	void Turn ( const FInputActionValue& InputValue );
	void InputJump ( const FInputActionValue& InputValue );
	void InputCrouch ( const FInputActionValue& InputValue );
	void MeleeAttack ( const FInputActionValue& InputValue );
	void PotatoAttack ( const FInputActionValue& InputValue );

	void CameraChange ( bool Value );
	void CameraChange ( const FInputActionValue& InputValue );
	
private:
	//감자 던지기 몽타주 동기화
	UFUNCTION ( Server , Reliable )
	void ServerRPC_AttackAnim ();
	UFUNCTION ( NetMulticast, Unreliable )
	void MultCastRPC_AttackAnim ();

	//감자 스폰
	UFUNCTION ( Server , Reliable )
	void ServerRPC_SpawnPotato (float Power);

	//감자 손에 들고 있는거 동기화
	UFUNCTION ( Server , Reliable )
	void ServerRPC_ShowRightPotato ( bool bShow );
	
	UFUNCTION ()
	void OnRep_ShowRightPotato ();

protected:
	UPROPERTY ( EditAnywhere ,  Category = Camera )
	class USpringArmComponent* SpringArmComp;
	UPROPERTY ( EditAnywhere ,  Category = Camera )
	class UCameraComponent* CameraComp;

	UPROPERTY ( EditDefaultsOnly , Category = Hunter )
	class USceneComponent* RightHandComp;

	UPROPERTY ( EditDefaultsOnly , Category = Hunter )
	class UStaticMeshComponent* RightPotatoComp;

	UPROPERTY ( EditDefaultsOnly , Category = Hunter )
	TSubclassOf<class APotato> PotatoClass;
	
	UPROPERTY()
	class UHunterAnimInstance* HunterAnimIns;

	UPROPERTY( VisibleAnywhere )
	bool bFirstPersonView = true;
	UPROPERTY( VisibleAnywhere )
	bool bLockCameraChange = false;

protected:
	//서버 동기화 변수들
	UPROPERTY ( ReplicatedUsing = OnRep_ShowRightPotato )
	bool bShowRightPotato = true;

};
