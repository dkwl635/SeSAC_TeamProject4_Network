// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogSlot.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UKillLogSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_HunterNick;
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_WitchNick;

public:
	void SetText ( FString HunterName , FString WitchName );
	
};
