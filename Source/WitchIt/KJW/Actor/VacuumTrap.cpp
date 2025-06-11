// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/VacuumTrap.h"

#include "KJW/GameMode/HASGameState.h"
#include "SSA/Character/Witch.h"
#include "SSA/Actor/ItemBase.h"

#include "NiagaraComponent.h"

#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KJW/Object/SFXGameInstanceSubsystem.h"

// Sets default values
AVacuumTrap::AVacuumTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	SetRootComponent ( MeshComp );

	MeshComp->SetSimulatePhysics ( true );
	MeshComp->SetNotifyRigidBodyCollision ( true );
	MeshComp->SetCollisionEnabled ( ECollisionEnabled::QueryAndPhysics );
	MeshComp->OnComponentHit.AddDynamic ( this , &AVacuumTrap::OnHitComponent );

	VacuumEffectComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "VacuumEffectComp" ) );
	VacuumEffectComp->SetupAttachment ( GetRootComponent() );
	VacuumEffectComp->SetVisibility ( false );
	VacuumEffectComp->bAutoActivate = false;
	
	SetLifeSpan (0.0f);

	//움직임 동기화
	bReplicates = true;
	SetReplicateMovement ( true );
}

// Called when the game starts or when spawned
void AVacuumTrap::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority ()) return;

	FVector ForceDir = GetActorForwardVector () * 20000.0f;
	MeshComp->AddImpulse ( ForceDir );

}



void AVacuumTrap::VacuumEffect ()
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "땡기기" ) );
	if (!GS)
		GS = GetWorld ()->GetGameState<AHASGameState> ();

	if (GS)
	{
		FindItems.Empty ();
	

		TArray<AItemBase*>& Items = GS->GetItems ();
		for (AItemBase* Item : Items)
		{
			float Dis = GetDistanceTo ( Item );
			if (Dis < VacuumDistance)
				FindItems.Add ( Item );
		}

			
		for (AItemBase* Item : FindItems)
		{
			if (!Item || !Item->IsValidLowLevel ()) continue;

			FVector Direction = (GetActorLocation() - Item->GetActorLocation()).GetSafeNormal ();
			Item->meshComp->AddForce ( Direction * 10000.0f, NAME_None , true );
		}


	}

}

void AVacuumTrap::VacuumCharacterEffect ()
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "땡기기" ) );
	if (!GS)
		GS = GetWorld ()->GetGameState<AHASGameState> ();

	if (GS)
	{
		FindWitchs.Empty ();

	
		TArray<ABaseCharacter*>& Witches = GS->GetWitches ();
		for (ABaseCharacter* Witch : Witches)
		{
			float Dis = GetDistanceTo ( Witch );
			if (Dis < VacuumDistance && Dis >10.0f)
				FindWitchs.Add ( Witch );
		}


		FVector TargetPos = GetActorLocation ();
		TargetPos.Z = TargetPos.Z + 100.0f;
		for (ABaseCharacter* Witch : FindWitchs)
		{
			if (!Witch || !Witch->IsValidLowLevel ()) continue;
			TargetPos.Z = Witch->GetActorLocation ().Z;
			FVector Direction = TargetPos - Witch->GetActorLocation ();
			Direction.Normalize ();
			//FVector NewPos = Witch->GetActorLocation () + Direction * 800.0f *0.02f;

			float Strength = 300.0f;
			Witch->LaunchCharacter ( Direction * Strength , true , true );
			//Witch->SetActorLocation ( NewPos );
			//Witch->ClientRPC_SetLocation ( Direction );
		}
	}
}


void AVacuumTrap::OnHitComponent ( UPrimitiveComponent* HitComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , FVector NormalImpulse , const FHitResult& Hit )
{
	//UE_LOG ( LogTemp , Warning , TEXT ( "%s") , *OtherActor->GetName());
	if (IsHit)
	{
		return;
	}

	USFXGameInstanceSubsystem* subsystem = GetGameInstance ()->GetSubsystem<USFXGameInstanceSubsystem> ();
	if (subsystem)
	{
		subsystem->PlaySound ( GetWorld () , GetActorLocation () , 2 );
	}


	if (!HasAuthority ()) return;

	
	
	IsHit = true;
	//VacuumEffectComp->SetVisibility ( true );
	//VacuumEffectComp->Activate ();

	MulticastRPC_ShowEffect ();

	MeshComp->SetSimulatePhysics ( false );

	

	GetWorldTimerManager ().SetTimer ( VacuumTimerHandle , this , &AVacuumTrap::VacuumEffect , 0.1f , true );
	GetWorldTimerManager ().SetTimer ( VacuumTimerCharacterHandle , this , &AVacuumTrap::VacuumCharacterEffect , 0.1f, true );
	
	if (bTest) return;
	FTimerHandle DestroyTimer;
	GetWorldTimerManager ().SetTimer ( DestroyTimer , FTimerDelegate::CreateLambda ( [this]() {
		this->Destroy ();
		} ) , 3.5f , false );

}

void AVacuumTrap::MulticastRPC_ShowEffect_Implementation ()
{
	VacuumEffectComp->SetVisibility ( true );
	VacuumEffectComp->Activate ();
}

