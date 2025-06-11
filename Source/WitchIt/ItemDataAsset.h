// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
USTRUCT (BlueprintType)
struct FMyAssetInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	FString ObjectName = TEXT("UnKnown Object");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	UStaticMesh* ObjectMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	float MaxHealth = 0.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object" )
	TSubclassOf<class AItemBase> ObjectClass;

};


UCLASS()
class WITCHIT_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FMyAssetInfo> AssetItems;

};
