// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerRecordBox.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UPlayerRecordBox : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_PlayerName;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Record;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_RecordTitle;


};
