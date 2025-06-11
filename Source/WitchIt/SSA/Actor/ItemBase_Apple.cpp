// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Actor/ItemBase_Apple.h"
#include "ItemBase_Apple.h"

AItemBase_Apple::AItemBase_Apple ()
{
	ItemName = "Apple";

}

void AItemBase_Apple::OnConstruction ( const FTransform& Transform )
{
	Super::OnConstruction ( Transform );
}

void AItemBase_Apple::BeginPlay ()
{
	Super::BeginPlay ();

}

void AItemBase_Apple::Tick ( float DeltaTime )
{
	Super::Tick ( DeltaTime );
}
