// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/Chicken.h"
#include "Components/SphereComponent.h"
#include "KJW/GameMode/HideAndSeekGameMode.h"
#include "KJW/GameMode/HASGameState.h"
#include "SSA/Character/Witch.h"
#include "NiagaraComponent.h"
#include "KJW/Object/SFXGameInstanceSubsystem.h"

// Sets default values
AChicken::AChicken()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	SetRootComponent ( SphereComp );


	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent> ( TEXT ( "MeshComp" ) );
	MeshComp->SetupAttachment ( GetRootComponent () );

	SphereComp->OnComponentHit.AddDynamic(this, &AChicken::OnHitComponent);

	SphereComp->SetSimulatePhysics ( true );
	SphereComp->SetNotifyRigidBodyCollision ( true );
	SphereComp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
	SphereComp->SetLinearDamping ( 0.01f );
	SphereComp->SetAngularDamping ( 30.0f );


	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetNotifyRigidBodyCollision ( false );
	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );


	FindEffectComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "FindEffectComp" ) );
	FindEffectComp->SetupAttachment ( GetRootComponent () );
	AuraEffectComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "AuraEffectComp" ) );
	AuraEffectComp->SetupAttachment ( GetRootComponent () );

	SetLifeSpan ( 10.0f );

	bReplicates = true;
	SetReplicateMovement ( true );
}

// Called when the game starts or when spawned
void AChicken::BeginPlay()
{
	Super::BeginPlay();
	

	USFXGameInstanceSubsystem* subsystem = GetGameInstance ()->GetSubsystem<USFXGameInstanceSubsystem> ();
	if (subsystem)
	{
		subsystem->PlaySound ( GetWorld () , GetActorLocation () , 1 );
	}



	if (!HasAuthority ()) return;

	FVector ForceDir = GetActorForwardVector () * 40000.0f;
	SphereComp->AddImpulse ( ForceDir );


	GetWorldTimerManager ().SetTimer ( FindWitchTimerHandle , this , &AChicken::FindWitchs , 0.3f , true );
	FindEffectComp->SetVisibility ( false );
}

// Called every frame
void AChicken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority ()) return;
	float Speed = SphereComp->GetComponentVelocity ().Size();
	

	if (Speed < 1.0f)
	{
		RecoverUpright ( DeltaTime );
	}

}

void AChicken::FindWitchs ()
{
	//if (!IsValid ( this ))
	//	return;

	if (!GS)
		GS = GetWorld ()->GetGameState<AHASGameState> ();

	

	if (GS)
	{

		bool IsNearWitch = false;

		TArray<ABaseCharacter*>& Witchs = GS->GetWitches ();
		for (ABaseCharacter* Witch : Witchs)
		{
			float Dis = GetDistanceTo ( Witch );
			if (Dis <= FindDistance)
				IsNearWitch = true;

			if (IsNearWitch)
				break;
		}

		MulticastRPC_ShowFindEffect ( IsNearWitch );
		
		if (IsNearWitch)
		{
			FindEffectComp->SetVisibility ( true );
		}
		else
		{
			FindEffectComp->SetVisibility ( false );
		}
	}

//	DrawDebugSphere ( GetWorld () , GetActorLocation () , FindDistance , 10 , FColor::Red  );

}

void AChicken::RecoverUpright ( float DeltaTime )
{
	if (!HasAuthority ()) return;

	FRotator CurRot = GetActorRotation ();
	FRotator NextRot = FRotator::ZeroRotator;

	NextRot = FMath::Lerp ( CurRot , NextRot , 5.0f * DeltaTime );

	SetActorRotation ( NextRot );
}

void AChicken::MulticastRPC_ShowFindEffect_Implementation ( bool bShow )
{
	FindEffectComp->SetVisibility ( bShow );
}


void AChicken::OnHitComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority ()) return;

	AWitch* Witch = Cast<AWitch>(OtherActor);
	if (Witch)
	{
		FVector CurrentVelocity = SphereComp->GetPhysicsLinearVelocity();

		// 속도를 절반으로 줄이기
		FVector ReducedVelocity = CurrentVelocity * 0.5f;

		SphereComp->SetPhysicsLinearVelocity(ReducedVelocity);
	}

}

