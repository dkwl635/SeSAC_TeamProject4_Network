// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SkillData.generated.h"

/**
 * 
 */
USTRUCT ( BlueprintType )
struct WITCHIT_API FSkillData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillIcon;

	UPROPERTY(EditAnywhere , BlueprintReadWrite )
	float CoolTime = 5.0f;

};
