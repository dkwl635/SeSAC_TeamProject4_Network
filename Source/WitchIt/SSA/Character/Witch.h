// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "KJW/Actor/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "../Actor/ItemBase.h"
#include "Witch.generated.h"

UCLASS()
class WITCHIT_API AWitch : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* WitchCameraComp; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPosition)
	class USceneComponent* LuaqPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* SkeltalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPosition)
	TSubclassOf<class AClingyLuaq> LuaqFactory;

	// 소품 변신을 위한 메시 컴포넌트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transform)
	class UStaticMeshComponent* TransformMeshComp;

	UPROPERTY(EditDefaultsOnly, Replicated, Category = Transform)
	class UDataTable* TransmformDataTable;

	FObjectData* object;

	
public:

	UPROPERTY()
	class UWitchAnim* WitchAnim;

public:
	void Move         ( const FInputActionValue& inputValue );
	void Turn         ( const FInputActionValue& inputValue );
	void InputJump    ( const FInputActionValue& inputValue );
	void InputCrouch  ( const FInputActionValue& inputValue );
	void Interact     ( const FInputActionValue& inputValue );
	void CameraChange ( const FInputActionValue& InputValue );


	void CameraChange ( bool Value );

	void StartFly    ();
	void  StopFly    ();

	void OnDamage (int32 damage) override;
	void Die ();


	void SkillLuaq();

	void TransformToObject( UStaticMesh* NewMesh, const FVector& NewRelativeLocation, float NewCapsuleRadius, float NewCapsuleHalfHeight, float NewitemHP, float HPRatio);
	void UnTransformToWitch();

	void FindTransformObject ();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemOutline( AItemBase* Item , bool bEnable );

private:
	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnLuaq();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SpawnLuaq();

	UFUNCTION(Server, Reliable)
	void ServerRPC_TransformToObject( UStaticMesh* NewMesh , const FVector& NewRelativeLocation , float NewCapsuleRadius , float NewCapsuleHalfHeight , float NewitemHP , float HPRatio );

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TransformToObject( UStaticMesh* NewMesh , const FVector& NewRelativeLocation , float NewCapsuleRadius , float NewCapsuleHalfHeight , float NewitemHP , float HPRatio );
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_UnTransformToWitch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UnTransformToWitch();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_StartFly();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StartFly();

	UFUNCTION(Server, Reliable)
	void ServerRPC_StopFly();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StopFly();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Die();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Die();

public:
	//bool bCrouched = false; 

	float Thrust = 0.0f;

public:
	FVector Direction;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = SpawnPosition)
	bool bIsFlying = false;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = SpawnPosition)
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPosition)
	float FlyManaConsumeRate = 50.0f;
	

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = SpawnPosition)
	float ManaRegenRate = 10.0f;
	
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = SpawnPosition)
	float FlyUpSpeed = 100.0f;

	UPROPERTY( Replicated )
	bool bIsTransform    = true;

	UPROPERTY( Replicated )
	bool bIsTransformDie = false;

	bool bFirstPersonView = false;



	UPROPERTY(Replicated)
	FVector OriginMeshRelativeLocation;

	UPROPERTY(Replicated)
	float OriginCurHP = 0.0f;

	UPROPERTY(Replicated)
	float OriginCapsuleRadius     = 0.0f;

	UPROPERTY(Replicated)
	float OriginCapsuleHalfHeight = 0.0f;

	FString HitName;

	UPROPERTY(Transient)
    AItemBase* LastHighlightedItem;

};
