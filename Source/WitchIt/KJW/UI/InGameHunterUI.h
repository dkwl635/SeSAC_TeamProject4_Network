// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHunterUI.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UInGameHunterUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	friend class AHunter;

public :
	UPROPERTY(BlueprintReadWrite , meta = (BindWidget))
	class UProgressBar* ProgressBar_HuterMana;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UProgressBar* ProgressBar_Skill1;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UProgressBar* ProgressBar_Skill2;
	//수색 . 숨기
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UKTextBlock* TextLabel_Time;
	// 00 : 00 
	UPROPERTY (BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Time;


	UPROPERTY (BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_Skill1Key;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_Skill2Key;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Skill1Cool;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Skill2Cool;

public:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;
private:
	void SetHunterUI ();
	void SetMana ();
	void SetTimer ();
	
	void ManaLerp ( float InDeltaTime );

	void CoolTimeProgressBarUpdate ();
private:

	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;

	UPROPERTY()
	class UStateComponent* StateComp;

	UPROPERTY()
	class USkillComponent* SkillComp;

	UPROPERTY()
	class AHASGameState* GS;

	float ManaCurentValue = 1.0f;
	float ManaLerpValue = ManaCurentValue;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true") )
	FText Text_HunterHide;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true") )
	FText Text_HunterSeek;
	UPROPERTY ( EditAnywhere , meta = (AllowPrivateAccess = "true") )
	FText Text_Ready;
};
