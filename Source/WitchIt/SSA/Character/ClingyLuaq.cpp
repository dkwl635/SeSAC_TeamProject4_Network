// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Character/ClingyLuaq.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/PrimitiveComponent.h"
#include "KJW/Actor/Hunter.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"


// Sets default values
AClingyLuaq::AClingyLuaq()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetSphereRadius ( 90.0f );
	

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(GetRootComponent());
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/SSA/Asset/charlotte-bunny/source/Walk.Walk'" ) );
	if(TempMesh.Succeeded())
	{
		MeshComp->SetSkeletalMesh ( TempMesh.Object );
		MeshComp->SetEnableGravity ( false );
		MeshComp->SetRelativeScale3D(FVector(50.0f));
		MeshComp->SetRelativeLocationAndRotation(FVector(-10.0f, 0.0f , -87.0f ), FRotator ( 0.0f , -90.0f , 0.0f ));
	}
	
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType( ECC_GameTraceChannel5);
	CollisionComp->SetCollisionResponseToChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel( ECC_GameTraceChannel1, ECR_Overlap);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MoveComp"));
	MovementComp->SetUpdatedComponent(CollisionComp);

	MovementComp->InitialSpeed = 1000.0f;
	//
	MovementComp->MaxSpeed = 1000.0f;

	bReplicates = true;
	SetReplicateMovement ( true );

	AuraEffectComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "AuraEffectComp" ) );
	AuraEffectComp->SetupAttachment ( GetRootComponent () );

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AClingyLuaq::OnLuaqOverlap);
}

// Called when the game starts or when spawned
void AClingyLuaq::BeginPlay()
{
	Super::BeginPlay();

	SetActorEnableCollision(true);
}

void AClingyLuaq::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );
}

// Called every frame
void AClingyLuaq::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClingyLuaq::OnLuaqOverlap ( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult )
{
	if (IsHit)  return;
	IsHit = true;

	Hunter = Cast<AHunter> ( OtherActor );

	if (Hunter != nullptr)
	{
		MovementComp->Deactivate();

		AttachToHunter(Hunter);

		GetWorldTimerManager().SetTimer( RemoveTimerHandle, this, &AClingyLuaq::Die, 3.0f, false);
		GEngine->AddOnScreenDebugMessage ( -1 , 2.0f , FColor::Blue , FString::Printf ( TEXT ( "충돌!" ) ) );
		MeshComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
}

void AClingyLuaq::AttachToHunter ( AActor* OtherActor )
{
	Hunter = Cast<AHunter>(OtherActor);

	if (Hunter != nullptr)
	{
		Hunter->HitLuaq ( true );
		UCameraComponent* Camera = Hunter->FindComponentByClass<UCameraComponent>();

		if (Camera != nullptr)
		{
			AttachToComponent(Camera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			SetActorRelativeLocation(FVector(70.0f, 0.0f, -15.0f));
			SetActorRelativeRotation(FRotator::ZeroRotator);
		}
	}
}

void AClingyLuaq::Die ()
{
	if (Hunter)
	{
		Hunter->HitLuaq ( false );
	}
		this->Destroy();
}

