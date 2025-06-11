// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerListUI.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UPlayerListUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UVerticalBox* VerticalBox_Witch;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UVerticalBox* VerticalBox_Hunter;

public:
	UFUNCTION()
	void SetPlayersInfo ();
private:
	virtual void NativeConstruct () override;
private:

	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class UPlayerBox> PlayerBoxClass;

	UPROPERTY ()
	TArray<class UPlayerBox*> PlayerInfoBoxes;

	UPROPERTY()
	class AHASGameState* GS;
private:

	class UPlayerBox* GetPlayerBox ();
	void HideAllBox ();

};
