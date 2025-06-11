// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/InGameHunterUI.h"
#include "KJW/Actor/BaseCharacter.h"
#include "KJW/ActorComponent/SkillComponent.h"
#include "SSA/ActorComponent/StateComponent.h"

#include "Slate/SlateBrushAsset.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "KJW/Widget/KTextBlock.h"
#include "KJW/GameMode/HASGameState.h"

void UInGameHunterUI::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerCharacter = GetOwningPlayerPawn<ABaseCharacter>();
	if (OwnerCharacter)
	{
		SetHunterUI();
	}

	GS = GetWorld()->GetGameState<AHASGameState>();
}

void UInGameHunterUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ManaLerp(InDeltaTime);
	CoolTimeProgressBarUpdate ();
	SetTimer();
}

void UInGameHunterUI::SetHunterUI()
{
	StateComp = OwnerCharacter->StateComp;
	SkillComp = OwnerCharacter->SkillComp;

	

	ManaCurentValue = 1.0f;
	ManaLerpValue = 1.0f;

	
	FProgressBarStyle NewStyle = ProgressBar_Skill1->GetWidgetStyle();
	FSlateBrush BackGrountSlate = NewStyle.BackgroundImage;
	BackGrountSlate.DrawAs = ESlateBrushDrawType::Image;
	BackGrountSlate.SetResourceObject ( SkillComp->GetSkill ( 0 )->SkillIcon );

	NewStyle.SetBackgroundImage( BackGrountSlate );
	ProgressBar_Skill1->SetWidgetStyle ( NewStyle );

	NewStyle = ProgressBar_Skill2->GetWidgetStyle ();
	BackGrountSlate = NewStyle.BackgroundImage;
	BackGrountSlate.DrawAs = ESlateBrushDrawType::Image;
	BackGrountSlate.SetResourceObject ( SkillComp->GetSkill ( 1 )->SkillIcon );

	NewStyle.SetBackgroundImage ( BackGrountSlate );
	ProgressBar_Skill2->SetWidgetStyle ( NewStyle );

	SetMana();

	StateComp->StateUpdateDelegate.AddUObject ( this , &UInGameHunterUI::SetMana );

}

void UInGameHunterUI::SetMana()
{
	if (!StateComp) return;

	float MaxMana = StateComp->GetMaxMana();
	float Mana = StateComp->GetCurMana();

	ManaCurentValue  = Mana / MaxMana;
}

void UInGameHunterUI::SetTimer()
{
	if (!GS)return;

	float Timer = 0;
	FText text = FText ();

	int32 Text_ID = 0;

	switch (GS->GetHASGameState ())
	{
	case  EHASGameState::READY:
	{
		text = Text_Ready;
		Timer = 0;
		Text_ID = 16;
		break;
	}
	case  EHASGameState::HIDE:
	{
		text = Text_HunterHide;
		Timer = GS->HideTimer;
		Text_ID = 18;
		break;
	}
	case  EHASGameState::SEEK:
	{
		text = Text_HunterSeek;
		Timer = GS->GameTimer;
		Text_ID = 20;
		break;
	}
	default:
		break;
	}
	TextLabel_Time->Text_ID = Text_ID;
	TextLabel_Time->UpdateTextFromTag ();
	//TextLabel_Time->SetText ( text );
	int32 min = Timer / 60;
	int32 sec = (int32)Timer % 60;

	FString str = FString::Printf ( TEXT ( "%02d : %02d" ) , min , sec );
	Text_Time->SetText ( FText::FromString ( str ) );
}



void UInGameHunterUI::ManaLerp(float InDeltaTime)
{
	ManaLerpValue = FMath::Lerp(ManaLerpValue, ManaCurentValue, 5.0f * InDeltaTime);

	float Percent = 1 - ManaLerpValue;
	ProgressBar_HuterMana->SetPercent(Percent);
}

void UInGameHunterUI::CoolTimeProgressBarUpdate ()
{
	//A 스킬
	USkillBase* Skill = SkillComp->GetSkill ( 0 );
	float SkillCoolTime = Skill->CoolTime;
	float SkillCoolTimer = Skill->CoolTimer;
	float percent = SkillCoolTimer / SkillCoolTime;

	int32 SkillCoolTimeint = (int32)Skill->CoolTimer + 1;

	if (SkillCoolTimer > 0.0f)
		Text_Skill1Cool->SetText ( FText::AsNumber ( SkillCoolTimeint ) );
	else
		Text_Skill1Cool->SetText ( FText () );

	ProgressBar_Skill1->SetPercent ( percent );

	//B 스킬
	Skill = SkillComp->GetSkill ( 1 );
	 SkillCoolTime = Skill->CoolTime;
	 SkillCoolTimer = Skill->CoolTimer;
	 percent = SkillCoolTimer / SkillCoolTime;

	 SkillCoolTimeint = (int32)Skill->CoolTimer + 1;

	 if (SkillCoolTimer > 0.0f)
		 Text_Skill2Cool->SetText ( FText::AsNumber ( SkillCoolTimeint ) );
	 else
		 Text_Skill2Cool->SetText ( FText () );
	ProgressBar_Skill2->SetPercent ( percent );
}
