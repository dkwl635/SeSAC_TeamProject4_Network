// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "KTextBlock.generated.h"

/**
 * 
 */
 // 데이터 테이블용 구조체 예시
USTRUCT ( BlueprintType )
struct FHASTextRow : public FTableRowBase
{
	GENERATED_BODY ()

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	int32 Text_ID = 0;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	FString Text_Kor;
	
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	FString Text_Eng;

};


UCLASS()
class WITCHIT_API UKTextBlock : public UTextBlock
{
	GENERATED_BODY()
	

public:
	UPROPERTY ( EditAnywhere  )
	int32 Text_ID;
public:
	UFUNCTION ()
	void UpdateTextFromTag ();
protected:
	virtual void OnWidgetRebuilt () override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty ( FPropertyChangedEvent& PropertyChangedEvent ) override;
#endif


};
