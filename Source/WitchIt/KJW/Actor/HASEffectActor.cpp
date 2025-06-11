// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Actor/HASEffectActor.h"
#include "NiagaraComponent.h"

// Sets default values
AHASEffectActor::AHASEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent> ( TEXT ( "NiagaraComp" ) );
	SetRootComponent ( NiagaraComp );
}

// Called when the game starts or when spawned
void AHASEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHASEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

