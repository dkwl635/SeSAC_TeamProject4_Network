// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWitchUI.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UInGameWitchUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UProgressBar* ProgressBar_WitchMana;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UProgressBar* ProgressBar_WitchHp;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_HP;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UProgressBar* ProgressBar_Skill1;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UProgressBar* ProgressBar_Skill2;

	// 00 : 00 
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Time;

	//수색 . 숨기
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UKTextBlock* TextLabel_Time;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_Skill1Key;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_Skill2Key;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Skill1Cool;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Skill2Cool;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_Transform;

	//죽었을때 나오는 잠깐 화면
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UCanvasPanel* Canvas_Die;
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_DieInfo;

public:
	virtual void NativeConstruct () override;
	virtual void NativeTick ( const FGeometry& MyGeometry , float InDeltaTime ) override;
private:
	void SetWitchUI ();
	void SetMana ();
	void SetHp ();
	void SetWitchMeshName ();

	void ManaLerp ( float InDeltaTime );
	void HpLerp ( float InDeltaTime );

	void CoolTimeProgressBarUpdate ();
	void SetTimer ();
	void SetTransform ();
	void DieEvent ( const FString& HunterNickName );
private:

	UPROPERTY ()
	class AWitch* OwnerWitch;

	UPROPERTY ()
	class UStateComponent* StateComp;

	UPROPERTY ()
	class USkillComponent* SkillComp;


	float ManaCurentValue = 1.0f;
	float ManaLerpValue = ManaCurentValue;
	

	float HpCurentValue = 1.0f;
	float HpLerpValue = HpCurentValue;
	
	UPROPERTY ()
	class AHASGameState* GS;

	UPROPERTY ( EditAnywhere , meta = (AllowPrivateAccess = "true") )
	FText Text_WitchHide;
	UPROPERTY ( EditAnywhere , meta = (AllowPrivateAccess = "true") )
	FText Text_WitchSeek;
	UPROPERTY ( EditAnywhere , meta = (AllowPrivateAccess = "true") )
	FText Text_Ready;


	bool bDie = false;
};
