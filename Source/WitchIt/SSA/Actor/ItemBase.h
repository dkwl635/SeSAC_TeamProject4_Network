// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

USTRUCT(BlueprintType)
struct FObjectData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	int32 ItemID = -1; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	FString ObjectName = TEXT("UnKnown Object");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	UStaticMesh* ObjectMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	float MaxHealth = 0.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	float CapsuleRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	float CapsuleHalfHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	FVector CapsuleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	TSubclassOf<class AItemBase> ObjectClass; 

};

UCLASS()
class WITCHIT_API AItemBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = DataTable)
	class UDataTable* data;

	UPROPERTY(EditDefaultsOnly, Category = DataTable)
	class UItemDataAsset* assets;
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	virtual void OnConstruction ( const FTransform& Transform ) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps )const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	FObjectData* object;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	int32 ObjectID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	class UStateComponent* StateComp; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	float MaxHealth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	float CapsuleRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	float CapsuleHalfHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	FVector CapsuleLocation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings" )
	//FVector WitchLocation; 
	
	class AWitch* witch;

	bool bIsWitch = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<class UPostProcessComponent> Outline; 


public:
	void ScanPlayer();

	void SetOutline ( bool Value );
	
};
