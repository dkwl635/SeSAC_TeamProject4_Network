// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/Wall.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );
	SetRootComponent ( MeshComp );

}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

