// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/SpawnPoint.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent> ( TEXT ( "BoxComp" ) );
	SetRootComponent ( BoxComp );


	BoxComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

}



