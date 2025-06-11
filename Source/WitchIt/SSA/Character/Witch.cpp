// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Character/Witch.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "WitchAnim.h"
#include "Math/Matrix.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "SSA/ActorComponent/WIInputComponent.h"
#include "SSA/ActorComponent/StateComponent.h"
#include "ClingyLuaq.h"
#include "Components/SceneComponent.h"
#include "../Actor/ItemBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "WitchIt.h"
#include "KJW/GameMode/HASGameState.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "KJW/Object/SFXGameInstanceSubsystem.h"

// Sets default values
AWitch::AWitch()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 40.0f, 80.0f));
	SpringArmComp->TargetArmLength = 300.0f; 
	SpringArmComp->bUsePawnControlRotation = true; 
	
	WitchCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("WitchCamera"));
	WitchCameraComp->SetupAttachment(SpringArmComp);
	WitchCameraComp->bUsePawnControlRotation = false; 

	LuaqPosition =CreateDefaultSubobject<USceneComponent>(TEXT("LuaqPosition"));
	LuaqPosition->SetupAttachment ( RootComponent );

	bUseControllerRotationYaw   = false;
	bUseControllerRotationPitch = false; 
	bUseControllerRotationRoll  = false; 

	//MovementComponent->bOrientRotationToMovement = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Creative_Characters_FREE/Skeleton_Meshes/SK_Animations.SK_Animations'" ) );

	if (TempMesh.Succeeded ())
	{
		GetMesh()->SetSkeletalMesh ( TempMesh.Object );
		GetMesh()->SetRelativeLocationAndRotation ( FVector ( 0.0f , 0.0f , -90.0f ) , FRotator ( 0.0f , -88.0f , 0.0f ) );
	}

	ConstructorHelpers::FClassFinder<UWitchAnim> TempAnimClass ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/SSA/Character/ABP_Witch.ABP_Witch_C'" ) );
	
	if (TempAnimClass.Succeeded ())
	{
		GetMesh()->SetAnimInstanceClass ( TempAnimClass.Class );
	}

	TransformMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TransformMeshComp"));
	TransformMeshComp->SetupAttachment(RootComponent); 
	TransformMeshComp->SetRelativeLocation(FVector::ZeroVector); 
	TransformMeshComp->SetVisibility(false); 
}

// Called when the game starts or when spawned
void AWitch::BeginPlay()
{
	ABaseCharacter::BeginPlay();
	
	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
	
		if (subsystem)
		{
			subsystem->AddMappingContext ( WitchInputComp->IMC_WitchIt , 0 );
		}
	}

	WitchAnim = Cast<UWitchAnim> ( GetMesh ()->GetAnimInstance () );

	// 초기값 저장 
	OriginMeshRelativeLocation = TransformMeshComp->GetRelativeLocation ();
	OriginCapsuleRadius        = GetCapsuleComponent ()->GetUnscaledCapsuleRadius ();
	OriginCapsuleHalfHeight    = GetCapsuleComponent ()->GetUnscaledCapsuleHalfHeight ();
	OriginCurHP                = StateComp->GetCurHP ();

	PRINTLOG ( TEXT ( "OriginCurHP: %d" ) , OriginCurHP );
}

void AWitch::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	ABaseCharacter::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( AWitch , OriginMeshRelativeLocation );
	DOREPLIFETIME ( AWitch , OriginCapsuleRadius        );
	DOREPLIFETIME ( AWitch , OriginCapsuleHalfHeight    );
	DOREPLIFETIME ( AWitch , OriginCurHP                );
	DOREPLIFETIME ( AWitch , TransmformDataTable        );
	DOREPLIFETIME ( AWitch , bIsTransform               );
	DOREPLIFETIME ( AWitch , bIsTransformDie            );
	DOREPLIFETIME ( AWitch , bIsFlying                  );
	DOREPLIFETIME ( AWitch , bIsFalling                 );
	DOREPLIFETIME ( AWitch , FlyUpSpeed                 );
}

// Called every frame
void AWitch::Tick ( float DeltaTime )
{
	ABaseCharacter::Tick ( DeltaTime );

	FindTransformObject ();

	if (bIsFlying)
	{
		WitchAnim->bIsFlying = true;
		//WitchAnim->bIsJumping = false;

		if (StateComp->IsManaEmpty ())
		{
			StopFly ();
			return;
		}

		FRotator FlyRotator = GetActorRotation ();
		FVector Forward = FlyRotator.Vector ();
		
		const FVector forward = FRotationMatrix ( FlyRotator ).GetScaledAxis ( EAxis::X );
		const FVector	   Up = FRotationMatrix ( FlyRotator ).GetScaledAxis ( EAxis::Z );
		 
		FVector FlyDirection = (Up + forward * 0.3f).GetSafeNormal ();
	
		AddMovementInput ( FlyDirection );

		StateComp->ConsumeMana ( FlyManaConsumeRate * DeltaTime );
	}
	else
	{	
		WitchAnim->bIsFlying = false;
		if (!bIsFlying)
		{
			StateComp->RegenMana ( ManaRegenRate * DeltaTime );
		}
	}
}

// Called to bind functionality to input
void AWitch::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
	ABaseCharacter::SetupPlayerInputComponent ( PlayerInputComponent );

	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent> ( PlayerInputComponent );

	if (IsValid ( EnhancedInputComponent ))
	{
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Move         , ETriggerEvent::Triggered , this, &AWitch::Move          );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Turn         , ETriggerEvent::Triggered , this, &AWitch::Turn          );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Jump         , ETriggerEvent::Started   , this, &AWitch::InputJump     );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Crouch       , ETriggerEvent::Started   , this, &AWitch::InputCrouch   );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Interact     , ETriggerEvent::Started   , this, &AWitch::Interact      );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Interact     , ETriggerEvent::Completed , this, &AWitch::Interact      );

		EnhancedInputComponent->BindAction ( WitchInputComp->IA_CameraChange , ETriggerEvent::Started   , this , &AWitch::CameraChange );
	}
}

void AWitch::Move ( const FInputActionValue& inputValue )
{
	FVector2D value = inputValue.Get<FVector2D> ();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation ();

		const FRotator YawRotation ( 0 , Rotation.Yaw , 0 );

		const FVector ForwardDirection = FRotationMatrix ( YawRotation ).GetUnitAxis ( EAxis::X );
		const FVector   RightDirection = FRotationMatrix ( YawRotation ).GetUnitAxis ( EAxis::Y );

		AddMovementInput ( ForwardDirection , value.X );
		AddMovementInput (   RightDirection , value.Y );
	}
}

void AWitch::Turn ( const FInputActionValue& inputValue )
{
	FVector2D scale = inputValue.Get<FVector2D> ();
	AddControllerPitchInput ( scale.Y );
	AddControllerYawInput   ( scale.X );
}

void AWitch::InputJump ( const FInputActionValue& inputValue )
{
	WitchAnim->bIsFlying = false;
	
	Jump ();
}

void AWitch::InputCrouch ( const FInputActionValue& inputValue )
{
	if (bIsCrouched)
	{
		UnCrouch ();
	}
	else
	{
		Crouch ();
	}
}

void AWitch::Interact ( const FInputActionValue& inputValue )
{
	if (bIsTransform)
	{
		UnTransformToWitch();
		return; 
	}

	FHitResult hitInfo; 

	FVector startPos = WitchCameraComp->GetComponentLocation();
	FVector   endPos = WitchCameraComp->GetComponentLocation() + WitchCameraComp->GetForwardVector() * 2000.0f;

	FCollisionQueryParams params;

	params.AddIgnoredActor(this);


	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);
	
	if (bHit == true) 
	{
		AActor* HitActor = hitInfo.GetActor ();

		if (HitActor && TransmformDataTable)
		{
			AItemBase* item = Cast<AItemBase>(HitActor);

			if (item)
			{
				HitName      			= item->ItemName;
				FVector location        = item->CapsuleLocation;
				float CapsuleRadius     = item->CapsuleRadius;
				float CapsuleHalfHeight = item->CapsuleHalfHeight;
				float WitchRatio        = StateComp->GetHPRatio();
				float ObjectMaxHp       = item->MaxHealth;
				float ItemHP            = ObjectMaxHp * WitchRatio;

				FName RowName ( *HitName );

				const FObjectData* Row = TransmformDataTable->FindRow<FObjectData> (RowName, TEXT (""));

				if (Row && Row->ObjectMesh)
				{
					TransformToObject ( Row->ObjectMesh , Row->CapsuleLocation, Row->CapsuleRadius, Row->CapsuleHalfHeight, Row->MaxHealth, WitchRatio);
					return;
				}
			}
		}
	}
	else
	{
		UnTransformToWitch ();
	}
}

void AWitch::CameraChange ( const FInputActionValue& InputValue )
{
	CameraChange ( !bFirstPersonView );
}

void AWitch::CameraChange ( bool Value )
{
	bFirstPersonView = Value;

	if (bFirstPersonView)
	{
		SpringArmComp->TargetArmLength = 0.f;
		SpringArmComp->SetRelativeLocation ( FVector ( 40.0f , -15.0f , 50.0f ) );
	}
	else
	{
		SpringArmComp->TargetArmLength = 300.f;
		SpringArmComp->SetRelativeLocation ( FVector ( 0.0f , 20.0f , 90.0f ) );
	}
}

void AWitch::StartFly ()
{
	//bIsFlying = true;
	WitchAnim->bIsJumping = false;

	if (StateComp->IsManaEmpty ()) return;

	ServerRPC_StartFly ();
}

void AWitch::StopFly ()
{
	//bIsFlying = false;
	WitchAnim->bIsJumping = false;

	ServerRPC_StopFly ();
	/*GetCharacterMovement()->SetMovementMode(MOVE_Falling);*/
}

void AWitch::OnDamage ( int32 damage )
{
	if (StateComp->GetCurHP () <= 0)
		return;

	StateComp->TakeDamage(damage);

	PRINTLOG ( TEXT ( "Damaged by %d | CurHP : %.1f" ) , damage , StateComp->GetCurHP () );
	
	if (StateComp->GetCurHP () <= 0)
		Die ();
}

void AWitch::Die ()
{
	if (AHASGameState* GS = GetWorld ()->GetGameState<AHASGameState>())
	{
		GS->DieWitch ( Cast<APlayerController> ( GetController () ) );
	}

	ServerRPC_Die ();
}

void AWitch::SkillLuaq()
{
	ServerRPC_SpawnLuaq ();
}

void AWitch::TransformToObject ( UStaticMesh* NewMesh ,const FVector& NewRelativeLocation , float NewCapsuleRadius , float NewCapsuleHalfHeight, float NewitemHP, float HPRatio)
{
	if (!NewMesh || !TransformMeshComp) return;

	if (StateComp->IsManaEmpty() || StateComp->GetCurMana () <= 50) return;

	ServerRPC_TransformToObject ( NewMesh , NewRelativeLocation ,NewCapsuleRadius , NewCapsuleHalfHeight , NewitemHP , HPRatio );

	bIsTransformDie = true; 
}

void AWitch::UnTransformToWitch ()
{
	if (!TransformMeshComp) return;

	HitName = FString ( TEXT ( "Witch" ) );

	ServerRPC_UnTransformToWitch ();

	bIsTransform    = false;
	bIsTransformDie = false;
}

void AWitch::FindTransformObject ()
{
	if (!GetController () || !(GetController ()->IsLocalPlayerController ()))
	{
		return;
	}


	FHitResult HitInfo;
	FVector StartPos = WitchCameraComp->GetComponentLocation ();
	FVector EndPos = StartPos + WitchCameraComp->GetForwardVector () * 2000.0f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor ( this );

	AItemBase* NewHighlightedItem = nullptr;

	if (GetWorld ()->LineTraceSingleByChannel ( HitInfo , StartPos , EndPos , ECC_Visibility , Params ))
	{
		if (AItemBase* Item = Cast<AItemBase> ( HitInfo.GetActor () ))
		{
			NewHighlightedItem = Item;
			if (TransmformDataTable)
			{
				if (const FObjectData* Row = TransmformDataTable->FindRow<FObjectData> ( *Item->ItemName , TEXT ( "" ) ))
				{
					if (Row->ObjectMesh)
					{
						Item->SetOutline ( true );
					}
				}
			}
		}
	}
	if (LastHighlightedItem != NewHighlightedItem)
	{
		if (LastHighlightedItem)
		{
			LastHighlightedItem->SetOutline ( false );
		}

		LastHighlightedItem = NewHighlightedItem;
	}
}

void AWitch::ServerRPC_SetItemOutline_Implementation ( AItemBase* Item , bool bEnable )
{
	if (Item)
	{
		Item->SetOutline ( bEnable );
	}
}

void AWitch::ServerRPC_SpawnLuaq_Implementation ()
{
	FVector LuaqVector;
	FRotator LuaqRot;

	GetPlayerViewport ( LuaqVector , LuaqRot );

	FTransform LuaqPos = LuaqPosition->GetComponentTransform ();
	FActorSpawnParameters spawnparm;

	spawnparm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 물리충돌 문제 있어도 무조건 스폰되게 하는 것 
	GetWorld ()->SpawnActor<AClingyLuaq> ( LuaqFactory , this->GetActorLocation () , LuaqRot , spawnparm );
  }

void AWitch::MulticastRPC_SpawnLuaq_Implementation ()
{

}

void AWitch::ServerRPC_TransformToObject_Implementation ( UStaticMesh* NewMesh , const FVector& NewRelativeLocation , float NewCapsuleRadius , float NewCapsuleHalfHeight , float NewitemHP , float HPRatio )
{
	PRINTLOG ( TEXT ( "NewMesh: %s, NewRelativeLocation: %.2f, %.2f, %.2f, NewCapsuleRadius: %.2f, NewCapsuleHalfHeight: %.2f, NewitemHP: %.2f, HPRatio: %.2f" ) , NewMesh ? *NewMesh->GetName () : TEXT ( "NewMesh Is NullPtr" ) , NewRelativeLocation.X , NewRelativeLocation.Y , NewRelativeLocation.Z ,
		NewCapsuleRadius , NewCapsuleHalfHeight , NewitemHP , HPRatio );
	
	MulticastRPC_TransformToObject ( NewMesh , NewRelativeLocation , NewCapsuleRadius , NewCapsuleHalfHeight , NewitemHP , HPRatio );
	
}

void AWitch::MulticastRPC_TransformToObject_Implementation ( UStaticMesh* NewMesh , const FVector& NewRelativeLocation , float NewCapsuleRadius , float NewCapsuleHalfHeight , float NewitemHP , float HPRatio )
{
	PRINTLOG ( TEXT ( "NewMesh: %s, NewRelativeLocation: %.2f, %.2f, %.2f, NewCapsuleRadius: %.2f, NewCapsuleHalfHeight: %.2f, NewitemHP: %.2f, HPRatio: %.2f" ) , NewMesh ? *NewMesh->GetName () : TEXT ( "NewMesh Is NullPtr" ) , NewRelativeLocation.X , NewRelativeLocation.Y , NewRelativeLocation.Z ,
		NewCapsuleRadius , NewCapsuleHalfHeight , NewitemHP , HPRatio );
	TransformMeshComp->SetStaticMesh ( NewMesh );
	TransformMeshComp->SetVisibility ( true );

	USFXGameInstanceSubsystem* subsystem = GetGameInstance ()->GetSubsystem<USFXGameInstanceSubsystem> ();
	if (subsystem)
	{
		subsystem->PlaySound ( GetWorld () , GetActorLocation () , 3 );


			subsystem->SpawnEffect ( GetWorld () , GetActorLocation () , FRotator::ZeroRotator , 1 );
		
	}
	//FlyEffectComp->SetVisibility(true);

	GetMesh ()->SetVisibility ( false );

	TransformMeshComp->SetRelativeLocation ( NewRelativeLocation );
	GetCapsuleComponent ()->SetCapsuleSize ( NewCapsuleRadius , NewCapsuleHalfHeight );

	// 아이템 비율 체력
	StateComp->SetHPFromRatio ( NewitemHP , HPRatio );

	TArray<USceneComponent*> meshes;
	GetMesh ()->GetChildrenComponents ( true , meshes );
	for (auto& mesh : meshes)
	{
		mesh->SetVisibility ( false );
	}
}

void AWitch::ServerRPC_UnTransformToWitch_Implementation ()
{
	MulticastRPC_UnTransformToWitch ();
}

void AWitch::MulticastRPC_UnTransformToWitch_Implementation ()
{
	TransformMeshComp->SetVisibility ( false );

	GetMesh ()->SetVisibility ( true );

	TArray<USceneComponent*> meshes;
	GetMesh ()->GetChildrenComponents ( true , meshes );
	for (auto& mesh : meshes)
	{
		mesh->SetVisibility ( true );
	}

	TransformMeshComp->SetRelativeLocation ( OriginMeshRelativeLocation );
	GetCapsuleComponent ()->SetCapsuleSize ( OriginCapsuleRadius , OriginCapsuleHalfHeight );

	if (!bIsTransformDie)
	{
		float ObjectHPRatio = StateComp->GetHPRatio ();  // 변신 상태의 체력 비율
		StateComp->SetHPFromRatio ( OriginCurHP , ObjectHPRatio );
	}
}

void AWitch::ServerRPC_StartFly_Implementation ()
{
	MulticastRPC_StartFly ();
}

void AWitch::MulticastRPC_StartFly_Implementation ()
{
	if (bIsFlying)
	{
		WitchAnim->bIsJumping = false;
	}

	bIsFlying = true;

	//FlyEffectComp->SetVisibility ( true );

	GetCharacterMovement ()->SetMovementMode ( MOVE_Flying );
	if (StateComp->GetCurMana () <= 0)
	{
		StopFly ();
	}
}

void AWitch::ServerRPC_StopFly_Implementation ()
{

	MulticastRPC_StopFly ();
}

void AWitch::MulticastRPC_StopFly_Implementation ()
{

	bIsFlying = false;
	//FlyEffectComp->SetVisibility ( false );

	GetCharacterMovement ()->SetMovementMode ( MOVE_Falling );
}

void AWitch::ServerRPC_Die_Implementation ()
{
	MulticastRPC_Die();
}

void AWitch::MulticastRPC_Die_Implementation ()
{
	if (bIsTransformDie)
	{
		UnTransformToWitch ();


	}

	GetCapsuleComponent ()->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	GetCharacterMovement ()->DisableMovement ();

	GetMesh ()->SetCollisionProfileName ( TEXT ( "Ragdoll" ) );
	GetMesh ()->SetSimulatePhysics ( true );

	//this->Destroy ();
}

