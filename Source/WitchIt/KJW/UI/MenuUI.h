// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUI.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta=(BindWidget))
	class UWidgetSwitcher* Switcher;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Continue;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Language;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Exit;
	UPROPERTY(meta=(BindWidget))

	//언어 설정
	class UButton* Button_Kor;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Eng;
private:
	virtual void NativeConstruct () override;


private:
	
	UFUNCTION()
	void Continue ();

	UFUNCTION()
	void Language ();

	UFUNCTION()
	void Exit ();

	UFUNCTION()
	void Change_Kor ();
	
	UFUNCTION()
	void Change_Eng ();


};
