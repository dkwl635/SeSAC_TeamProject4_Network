// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSA/Actor/ItemBase.h"
#include "ItemBase_Apple.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API AItemBase_Apple : public AItemBase
{
	GENERATED_BODY()
	
public:
	AItemBase_Apple ();

protected:
	virtual void OnConstruction ( const FTransform& Transform ) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay () override;

public:
	// Called every frame
	virtual void Tick ( float DeltaTime ) override;

};
