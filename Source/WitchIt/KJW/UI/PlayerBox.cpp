// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/PlayerBox.h"
#include "PlayerBox.h"
#include "KJW/GameMode/HASPlayerState.h"
#include "KJW/Object/SkillBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"





void UPlayerBox::SetPlayerBox ( UTexture2D* PlayerIcon , FColor TextColor , FString PlayerStr , int32 PlayerPing , UTexture2D* SkillAImg , UTexture2D* SkillBImg )
{
	Img_PlayerIcon->SetBrushFromTexture ( PlayerIcon );

	
	FString str = FString::Printf ( TEXT ( "%s (%dms)" ) , *PlayerStr , PlayerPing );
	Text_PlayerInfo->SetText ( FText::FromString ( str ) );
	Text_PlayerInfo->SetColorAndOpacity ( FSlateColor ( TextColor ) );

	Img_PlayerSkillA->SetBrushFromTexture ( SkillAImg );
	Img_PlayerSkillB->SetBrushFromTexture ( SkillBImg );

}

void UPlayerBox::SetPlayerBox ( AHASPlayerState* PS )
{
	bool bWitch = PS->CharacterType == 1 ? false : true;

	UTexture2D* PlayerIcon = bWitch ? Img_WitchIcon : Img_HunterIcon;
	FColor TextColor = bWitch ? Color_Witch : Color_Hunter;

	//FString PlayerName = "Player" + FString::FromInt(i);
	FString PlayerName = PS->PlayerNickName;
	int32 PlayerPing = PS->GetPingInMilliseconds ();
	USkillBase* DefaultSkill = (bWitch ? WitchSkills[PS->SkillA] : HunterSkills[PS->SkillA])->GetDefaultObject<USkillBase> ();

	UTexture2D* SkillA = DefaultSkill->SkillIcon;

	DefaultSkill = (bWitch ? WitchSkills[PS->SkillB] : HunterSkills[PS->SkillB])->GetDefaultObject<USkillBase> ();

	UTexture2D* SkillB = DefaultSkill->SkillIcon;

	Img_PlayerIcon->SetBrushFromTexture ( PlayerIcon );


	FString str = FString::Printf ( TEXT ( "%s (%dms)" ) , *PlayerName , PlayerPing );
	Text_PlayerInfo->SetText ( FText::FromString ( str ) );
	Text_PlayerInfo->SetColorAndOpacity ( FSlateColor ( TextColor ) );

	Img_PlayerSkillA->SetBrushFromTexture ( SkillA );
	Img_PlayerSkillB->SetBrushFromTexture ( SkillB );
}
