// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PropsBase.generated.h"


USTRUCT ( BlueprintType )
struct FPropData : public FTableRowBase
{
	GENERATED_BODY ()

public:

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "Object" )
	FString ObjectName = TEXT ( "UnKnown Object" );

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "Object" )
	UStaticMesh* ObjectMesh = nullptr;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = "Object" )
	float MaxHealth = 0.0f;


};

UCLASS()
class WITCHIT_API APropsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	FDataTableRowHandle MyDataHandle;
	
	// Sets default values for this actor's properties
	APropsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction ( const FTransform& Transform ) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite )
	UStaticMeshComponent* MeshComp;

};
