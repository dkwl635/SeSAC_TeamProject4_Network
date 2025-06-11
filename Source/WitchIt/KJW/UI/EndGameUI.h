// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameUI.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UEndGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Button_Menu;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Button_Retry;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UKTextBlock* Text_Result;
	
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UKTextBlock* Text_LevelMove;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_NextTimer;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UCanvasPanel* Canvas_Result;

	UPROPERTY ( meta = (BindWidget) )
	class UMenuUI* WBP_Menu;

protected:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;

	UFUNCTION()
	void RetryGame ();

	void OpenResultUI ();

	void SetTimer ();

	UFUNCTION()
	void OpenMenu ();

private:

	class AHASGameState* GS;
	
};
