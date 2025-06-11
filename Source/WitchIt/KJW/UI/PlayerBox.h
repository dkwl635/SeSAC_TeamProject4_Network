// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerBox.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UPlayerBox : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_PlayerIcon;
	
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UTextBlock* Text_PlayerInfo;

	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_PlayerSkillA;
	
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UImage* Img_PlayerSkillB;

public:
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

public:
	void SetPlayerBox ( UTexture2D* PlayerIcon,FColor TextColor , FString PlayerStr , int32 PlayerPing , UTexture2D* SkillAImg , UTexture2D* SkillBImg );

	void SetPlayerBox (class AHASPlayerState* PS  );
};
