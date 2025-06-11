// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemPrimaryDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UItemPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

	UPROPERTY ( EditAnywhere )
	FString AssetName;

	UPROPERTY ( EditAnywhere )
	UTexture2D* AssetThumbnail;

	UPROPERTY ( EditAnywhere )
	UStaticMesh* AssetStaticMesh;

	UPROPERTY ( EditAnywhere )
	float MaxHP = 0.0f;

	//GetPrimaryAssetId() const override { return FPrimaryAssetId ( "AssetItems" , GetFName () ); }
};
