// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectTeamUI.generated.h"

/**
 * 
 */


UENUM ( BlueprintType )
enum class EHASCharacterType : uint8
{
	NONE  = 0,
	HUNTER  = 1,
	WITCH  = 2,

};


UCLASS()
class WITCHIT_API USelectTeamUI : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UWidgetSwitcher* WidgetSwitcher_WitchIt;
	
//팀 선택 화면
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Btn_Witch;
	
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Btn_Hunter;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Button_Menu;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_WitchPlayer;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_HunterPlayer;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_PlayerName;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_AllPlayer;

	UPROPERTY ( meta = (BindWidget) )
	class UMenuUI* WBP_Menu;

public:
//팀 선택 후 화면	
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Btn_ChangeTeam;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UButton* Btn_Ready;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_TeamBanner;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UVerticalBox* VerticalBox_Team;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* TextBlock_Skill;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_SkillA;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_SkillB;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_TopWitchPlayer;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_TopHunterPlayer;

public:
	virtual void NativeConstruct () override;

	void SetPlayerNickName ( const FString& PlayerNickName );

private:
	UFUNCTION()
	void SelectWitch ();
	UFUNCTION()
	void SelectHunter ();
	UFUNCTION()
	void BackToSelect ();
	UFUNCTION ()
	void Ready ();
	UFUNCTION()
	void OpenMenu ();

	void ChangeCharacter (TSubclassOf<class ABaseCharacter> CharacterClass );
	void ChangeTeamUI ();
	void ShowPlayerInfo ();
	
	class UPlayerBox* ShowPlayerBox ();
	void OffPlayerBoxes ();
	void SetPlayerState ();

	void ChangeCharacterType ( EHASCharacterType Type );
private:
	bool bWitch = false;

	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	FColor Color_Witch;

	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	FColor Color_Hunter;
	
	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	UTexture2D* Img_WitchBanner;
	
	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	UTexture2D* Img_HunterBanner;
	
	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	TArray<TSubclassOf<class USkillBase>> HunterSkills;

	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	TArray<TSubclassOf<class USkillBase>> WitchSkills;

	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	UTexture2D* Img_WitchIcon;
	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	UTexture2D* Img_HunterIcon;

	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	TSubclassOf<class UPlayerBox> PlayerBoxClass;
	
	UPROPERTY()
	TArray<class UPlayerBox*> PlayerInfoBoxes;

	UPROPERTY ()
	class 	AHASGameState* GS;

	int32 SkillA_Index = 0;
	int32 SkillB_Index = 1;
};
