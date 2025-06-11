// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Actor/Potato.h"
#include "SSA/Character/Witch.h"
#include "KJW/GameMode/HASGameState.h"
#include "SSA/ActorComponent/StateComponent.h"
#include "KJW/Object/SFXGameInstanceSubsystem.h"


// Sets default values
APotato::APotato()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	SetRootComponent ( MeshComp );


	MeshComp->OnComponentHit.AddDynamic ( this , &APotato::OnHitComponent );
	//OnComponentHit 이벤트 발생을 위해서는 물리를 켜야함
	MeshComp->SetSimulatePhysics ( true );
	//우선 중력은 무시
	MeshComp->SetEnableGravity ( true );

	MeshComp->SetNotifyRigidBodyCollision ( true );
	MeshComp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
	MeshComp->SetCollisionObjectType ( ECC_PhysicsBody );

	//서버 동기화
	bReplicates = true; 
	SetReplicateMovement ( true );
}

// Called when the game starts or when spawned
void APotato::BeginPlay()
{
	Super::BeginPlay();

	
}

void APotato::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	//DOREPLIFETIME (APotato , )

}

// Called every frame
void APotato::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector moveDirection = GetActorForwardVector (); 	
	//FVector newLocation = GetActorLocation () + moveDirection * MoveSPD * DeltaTime;
	//
	//SetActorLocation ( newLocation );

}



void APotato::SetPower ( float NewPower )
{
	Power = NewPower;

	FVector ForceDir = GetActorForwardVector () * 30000.0f * Power;
	MeshComp->AddImpulse ( ForceDir );
}



void APotato::OnHitComponent ( UPrimitiveComponent* HitComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector NormalImpulse , const FHitResult& Hit )
{
	AWitch* Witch = Cast<AWitch> ( OtherActor );

	USFXGameInstanceSubsystem* subsystem = GetGameInstance ()->GetSubsystem<USFXGameInstanceSubsystem> ();
	if (subsystem)
	{
		subsystem->PlaySound ( GetWorld () , GetActorLocation () , 0 );
		
		if (Witch)
		{
			subsystem->SpawnEffect ( GetWorld () , GetActorLocation () , FRotator::ZeroRotator , 0 );
		}
	}


	if (!HasAuthority ())return;

	
	if (Witch)
	{	
		if (Witch->StateComp->GetCurHP () > 0)
		{
			FString HunterNick = OwnerHunter->OwnerNickName;
			FString WitchNick = Witch->OwnerNickName;
			Witch->OnDamage ( Damage );

			if (Witch->StateComp->GetCurHP () <= 0.0f)
			{
				AHASGameState* GS = GetWorld()->GetGameState<AHASGameState> ();
				if (GS)
					GS->DieWitchLog ( HunterNick , WitchNick );
			}

		}
	}


	GEngine->AddOnScreenDebugMessage ( 0 , 1.0f , FColor::Red , OtherActor->GetActorLabel() );
	
	
	Destroy (  );
}

