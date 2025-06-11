// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/Hunter.h"
#include "Net/UnrealNetwork.h"

#include "SSA/ActorComponent/WIInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KJW/Actor/Potato.h"
#include "KJW/Object/HunterAnimInstance.h"
#include "SSA/ActorComponent/StateComponent.h"
#include "KJW/UI/InGameHunterUI.h"

// Sets default values
AHunter::AHunter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent> ( TEXT ( "SpringArmComp" ) );
	SpringArmComp->SetupAttachment ( RootComponent );
	SpringArmComp->SetRelativeLocation ( FVector ( 0.0f , 60.0f , 80.0f ) );
	SpringArmComp->TargetArmLength = 300.0f;
	//위아래 카메라를 위해서
	SpringArmComp->bUsePawnControlRotation = true; 

	CameraComp = CreateDefaultSubobject<UCameraComponent> ( TEXT ( "CameraComp" ) );
	CameraComp->SetupAttachment ( SpringArmComp );
	CameraComp->SetOrthoNearClipPlane ( 10.0f );
	//CameraComp->bUsePawnControlRotation = false; 

	
	bUseControllerRotationPitch = false; 
	//좌우 회전 을 위한
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	RightHandComp = CreateDefaultSubobject<USceneComponent> ( TEXT ( "RightHandComp" ) );
	RightHandComp->SetupAttachment ( GetMesh (), TEXT("RightHand"));
	
	RightPotatoComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "RightPotatoComp" ) );
	RightPotatoComp->SetupAttachment ( RightHandComp );
	RightPotatoComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
}

void AHunter::BeginPlay ()
{
	ABaseCharacter::BeginPlay ();

		CameraChange ( true );
	
	if (IsLocallyControlled())
		CameraChange ( true );
	else
		CameraChange ( false );
	

	HunterAnimIns =Cast<UHunterAnimInstance>( GetMesh ()->GetAnimInstance ());


}

void AHunter::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	ABaseCharacter::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( AHunter , bShowRightPotato );

}

void AHunter::Tick ( float DeltaTime )
{
	ABaseCharacter::Tick ( DeltaTime );

	float CurMana = StateComp->GetCurMana ();
	float RegenMana = 15.f;
	if (CurMana > 70.f)
		RegenMana *= 1.7f;
	else if(CurMana > 30.0f)
		RegenMana *= 1.3f;

	StateComp->RegenMana ( RegenMana * DeltaTime );
	
}

void AHunter::SetupPlayerInputComponent ( UInputComponent* PlayerInputComponent )
{
	ABaseCharacter::SetupPlayerInputComponent ( PlayerInputComponent );

	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent> ( PlayerInputComponent );

	if (IsValid ( EnhancedInputComponent ))
	{
		
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Move , ETriggerEvent::Triggered , this , &AHunter::Move );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Turn , ETriggerEvent::Triggered , this , &AHunter::Turn );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Jump , ETriggerEvent::Started , this , &AHunter::InputJump );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Crouch , ETriggerEvent::Started , this , &AHunter::InputCrouch );

		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Interact , ETriggerEvent::Started , this , &AHunter::PotatoAttack );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_CameraChange , ETriggerEvent::Started , this , &AHunter::CameraChange );


	}
}

void AHunter::Move ( const FInputActionValue& inputValue )
{
	FVector2D value = inputValue.Get<FVector2D> ();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation ();

		const FRotator YawRotation ( 0 , Rotation.Yaw , 0 );

		const FVector ForwardDirection = FRotationMatrix ( YawRotation ).GetUnitAxis ( EAxis::X );
		const FVector   RightDirection = FRotationMatrix ( YawRotation ).GetUnitAxis ( EAxis::Y );

		AddMovementInput ( ForwardDirection , value.X );
		AddMovementInput ( RightDirection , value.Y );
	}
}

void AHunter::Turn ( const FInputActionValue& inputValue )
{
	FVector2D scale = inputValue.Get<FVector2D> ();
	AddControllerPitchInput ( scale.Y );
	AddControllerYawInput ( scale.X );
}

void AHunter::InputJump ( const FInputActionValue& inputValue )
{
	ACharacter::Jump ();
}

void AHunter::InputCrouch ( const FInputActionValue& inputValue )
{
}

void AHunter::MeleeAttack ( const FInputActionValue& InputValue )
{

}

void AHunter::PotatoAttack ( const FInputActionValue& InputValue )
{
	//입력 딜레이 줄 필요 있음
	//또는 공격 몽타주 실행시
	if (!HunterAnimIns) return;

	//마나 체크 및 감소
	if (StateComp->GetCurMana () <= 5.0f) return;

	float NeedMana = StateComp->GetCurMana ();
	NeedMana = FMath::Clamp ( NeedMana , 5.0f , 15.0f );

	float Power = NeedMana / 15.0f;

	StateComp->ConsumeMana ( NeedMana );
	
	//모든 클라이언트 동기화
	//HunterAnimIns->PlayshootPotato ();
	ServerRPC_AttackAnim ();
		
	//서버로
	ServerRPC_SpawnPotato (  Power );
}

void AHunter::CameraChange ( const FInputActionValue& InputValue )
{
	CameraChange ( !bFirstPersonView );
}

void AHunter::OnRep_ShowRightPotato ()
{
	RightPotatoComp->SetVisibility ( bShowRightPotato );
}

void AHunter::ServerRPC_ShowRightPotato_Implementation ( bool bShow )
{
	bShowRightPotato = bShow;
}

void AHunter::ServerRPC_SpawnPotato_Implementation ( float Power )
{
	SpawnPotato ( Power );
}

void AHunter::ServerRPC_AttackAnim_Implementation ()
{
	MultCastRPC_AttackAnim ();
}

void AHunter::MultCastRPC_AttackAnim_Implementation ()
{
	HunterAnimIns->PlayshootPotato ();
}

void AHunter::CameraChange ( bool Value )
{
	if (bLockCameraChange) return;

	bFirstPersonView = Value;

	if (bFirstPersonView)
	{
		SpringArmComp->TargetArmLength = 0.f;
		SpringArmComp->SetRelativeLocation ( FVector ( 40.0f , -15.0f , 50.0f ) );
	}
	else
	{
		SpringArmComp->TargetArmLength = 300.f;
		//SpringArmComp->SetRelativeLocation ( FVector ( 0.0f , 20.0f , 0.0f ) );

		SpringArmComp->SetRelativeLocation ( FVector ( 0.0f , 20.0f , 90.0f ) );

	}
}

void AHunter::SpawnPotato (float Power)
{
	//카메라(플레이어 시점에서) 앞에 있는 방향으로 감자를 던지는 느낌으로
	//카메라 시점에서 가장 앞에 있는 물건이 타겟팅 되도록 설정
	//감자를 오른손에서 스폰후 타겟팅 된 방향으로 날아가게 하기
	ShowRightPotato ( false );

	FVector End = GetTraceFromView ();
	//무조건 스폰되게
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//오른손에서 나가게
	FVector SpawnPos = RightHandComp->GetComponentLocation ();
	//오른손에거 목적지 방향 구하기
	FVector Dir = End - SpawnPos;
	Dir.Normalize ();
	FRotator SpawnRot = Dir.Rotation ();

	APotato* Potato = GetWorld ()->SpawnActor<APotato> ( PotatoClass , SpawnPos , SpawnRot , SpawnParam );
	Potato->OwnerHunter = this;
	Potato->SetPower ( Power );
}

void AHunter::ShowRightPotato ( bool bShow )
{
	bShowRightPotato = bShow;
	RightPotatoComp->SetVisibility ( bShow );

	if (IsLocallyControlled ())
		ServerRPC_ShowRightPotato ( bShow );
}

void AHunter::HitLuaq(bool IsStart)
{
	if (IsStart)
	{
		CameraChange ( IsStart );
		bLockCameraChange = IsStart;
	}
	else
	{
		bLockCameraChange = IsStart;
		//CameraChange ( IsStart );
	}
	
}

FVector AHunter::GetTraceFromView ()
{
	//카메라(플레이어 시점에서) 앞에 있는 방향
	// 1. 라인트레이스 세팅
	FVector Start;
	FRotator ViewRot;
	GetPlayerViewport ( Start , ViewRot );
	FVector End = Start + ViewRot.Vector () * 20000.0f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor ( this ); // 자기 자신은 무시

	//맞은 포인트가 있다면
	bool IsHit = GetWorld ()->LineTraceSingleByChannel ( HitResult , Start , End , ECC_Visibility , Params );
	if (IsHit)
	{
		//GEngine->AddOnScreenDebugMessage ( 1 , 1.0f , FColor::Red , HitResult.GetActor ()->GetActorLabel () );
		End = HitResult.Location;
	}

	return End;
}

FVector AHunter::GetRightHandLocation ()
{
	return RightHandComp->GetComponentLocation ();
}
