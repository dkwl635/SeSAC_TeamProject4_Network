// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Actor/PropsBase.h"

// Sets default values
APropsBase::APropsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( "MeshComp" );
	SetRootComponent ( MeshComp );

}

// Called when the game starts or when spawned
void APropsBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APropsBase::OnConstruction ( const FTransform& Transform )
{
	Super::OnConstruction ( Transform );
	if (MyDataHandle.IsNull ()) return;



	FPropData* data = MyDataHandle.GetRow<FPropData> ( TEXT ( "" ));
	if (data)
	{
		if (data->ObjectMesh)
			MeshComp->SetStaticMesh ( data->ObjectMesh );
	}


}

// Called every frame
void APropsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

