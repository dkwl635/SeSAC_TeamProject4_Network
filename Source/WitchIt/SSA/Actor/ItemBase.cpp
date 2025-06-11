// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Actor/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "SSA/ActorComponent/StateComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"
#include "../Character/Witch.h"
#include "Components/PostProcessComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SphereComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "SphereComp" ) );
	//SetRootComponent(SphereComp);
	//SphereComp->SetSphereRadius ( 200.0f );

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT( "MeshComp"));
	meshComp->SetupAttachment ( RootComponent );
	meshComp->SetEnableGravity ( true );
	meshComp->SetSimulatePhysics ( true );
	
	StateComp = CreateDefaultSubobject<UStateComponent> ( TEXT ( "StateComp" ) );

	Outline = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Outline"));
	Outline->SetupAttachment ( meshComp );
	Outline->bEnabled = false; 

}

void AItemBase::OnConstruction ( const FTransform& Transform )
{
	Super::OnConstruction ( Transform );

	if (object = data->FindRow<FObjectData> ( *ItemName , FString ( "" ) ))
	{
		meshComp->SetStaticMesh ( object->ObjectMesh );
	}
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	SetOutline ( false );

	meshComp->SetRenderCustomDepth ( false );

	// 값 셋팅
	if (object = data->FindRow<FObjectData> ( *ItemName , FString ( "" ) ))
	{
		ObjectID  = object->ItemID;
		MaxHealth = object->MaxHealth;
		meshComp->SetStaticMesh ( object->ObjectMesh );
	}
}

void AItemBase::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );


	//DOREPLIFETIME ( AItemBase , ObjectID          );
	//DOREPLIFETIME ( AItemBase , ItemName          );
	//DOREPLIFETIME ( AItemBase , meshComp          );
	//DOREPLIFETIME ( AItemBase , MaxHealth         );
	//DOREPLIFETIME ( AItemBase , CapsuleRadius     );
	//DOREPLIFETIME ( AItemBase , CapsuleHalfHeight );
	//DOREPLIFETIME ( AItemBase , CapsuleLocation   );
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsWitch && witch && FVector::Dist ( GetActorLocation () , witch->GetActorLocation () ) < 1000.f)
	{
		ScanPlayer ();
	}
}

void AItemBase::ScanPlayer ()
{
	if (witch)
	{
		FVector WitchLocation = Cast<AWitch>(witch)->GetActorLocation();

		FHitResult HitResult; 
		FCollisionQueryParams Params; 
		Params.AddIgnoredActor ( this );

		GetWorld ()->LineTraceSingleByChannel( HitResult , GetActorLocation () , WitchLocation , ECC_Visibility , Params );

		if (Cast<AWitch> ( HitResult.GetActor () ))
		{
			meshComp->SetRenderCustomDepth ( true );
		}
	}
}

void AItemBase::SetOutline ( bool Value )
{
	meshComp->SetRenderCustomDepth ( Value );
}

