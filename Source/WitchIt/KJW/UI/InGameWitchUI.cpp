// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/InGameWitchUI.h"
#include "SSA/Character/Witch.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "KJW/Widget/KTextBlock.h"
#include "KJW/ActorComponent/SkillComponent.h"
#include "SSA/ActorComponent/StateComponent.h"

#include "KJW/GameMode/HASGameState.h"



void UInGameWitchUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	OwnerWitch = GetOwningPlayerPawn<AWitch> ();
	if (OwnerWitch)
	{
		//StateUpdateDelegate
		SetWitchUI ();

	
	}

	GS = GetWorld()->GetGameState<AHASGameState>();
}

void UInGameWitchUI::NativeTick ( const FGeometry& MyGeometry , float InDeltaTime )
{
	Super::NativeTick ( MyGeometry , InDeltaTime );
	
	ManaLerp ( InDeltaTime );
	HpLerp ( InDeltaTime );
	SetTimer();
	SetWitchMeshName ();
	//CoolTimeProgressBarUpdate ();
}

void UInGameWitchUI::SetWitchUI ()
{

	StateComp = OwnerWitch->StateComp;
	SkillComp = OwnerWitch->SkillComp;

	ManaCurentValue = 1.0f;
	ManaLerpValue = 1.0f;


	FProgressBarStyle NewStyle = ProgressBar_Skill1->GetWidgetStyle ();
	FSlateBrush BackGrountSlate = NewStyle.BackgroundImage;
	BackGrountSlate.DrawAs = ESlateBrushDrawType::Image;
	BackGrountSlate.SetResourceObject ( SkillComp->GetSkill ( 0 )->SkillIcon );

	NewStyle.SetBackgroundImage ( BackGrountSlate );
	ProgressBar_Skill1->SetWidgetStyle ( NewStyle );

	NewStyle = ProgressBar_Skill2->GetWidgetStyle ();
	BackGrountSlate = NewStyle.BackgroundImage;
	BackGrountSlate.DrawAs = ESlateBrushDrawType::Image;
	BackGrountSlate.SetResourceObject ( SkillComp->GetSkill ( 1 )->SkillIcon );

	NewStyle.SetBackgroundImage ( BackGrountSlate );
	ProgressBar_Skill2->SetWidgetStyle ( NewStyle );

	SetMana ();
	SetHp ();
	
	StateComp->StateUpdateDelegate.AddUObject ( this , &UInGameWitchUI::SetMana );
	StateComp->StateUpdateDelegate.AddUObject ( this , &UInGameWitchUI::SetHp );


	Text_Skill1Cool->SetText ( FText () );
	Text_Skill2Cool->SetText ( FText () );
	ProgressBar_Skill1->SetPercent ( 0.0f);
	ProgressBar_Skill2->SetPercent ( 0.0f);
}

void UInGameWitchUI::SetMana ()
{
	if (!StateComp) return;

	float MaxMana = StateComp->GetMaxMana ();
	float Mana = StateComp->GetCurMana ();

	//Text_Mana->SetText ( FText::AsNumber ( (int32)Hp ) );
	ManaCurentValue = Mana / MaxMana;

}

void UInGameWitchUI::SetHp ()
{
	if (!StateComp) return;

	float MaxHp = StateComp->GetMaxHP ();
	float Hp = StateComp->GetCurHP ();

	if (Hp <= 0)
	{
		Hp = 0;

		//임시 나중에 서버에서 Die 이벤트를 캐릭터를 통해서 전달 받아야 함
		if (!bDie)
		{
			FString Test = "Hunter123";
			DieEvent ( Test );
		}
	}

	Text_HP->SetText ( FText::AsNumber ( (int32)Hp ) );
	HpCurentValue = Hp / MaxHp;
}

void UInGameWitchUI::SetWitchMeshName ()
{
	if (OwnerWitch)
	{
		Text_Transform->SetText(FText::FromString(OwnerWitch->HitName));
	}
}

void UInGameWitchUI::ManaLerp ( float InDeltaTime )
{
	ManaLerpValue = FMath::Lerp ( ManaLerpValue , ManaCurentValue , 5.0f * InDeltaTime );

	float Percent = ManaLerpValue;
	ProgressBar_WitchMana->SetPercent ( Percent );
}

void UInGameWitchUI::HpLerp ( float InDeltaTime )
{
	HpLerpValue = FMath::Lerp ( HpLerpValue , HpCurentValue , 5.0f * InDeltaTime );

	float Percent = HpLerpValue;
	ProgressBar_WitchHp->SetPercent ( Percent );
}

void UInGameWitchUI::CoolTimeProgressBarUpdate ()
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

void UInGameWitchUI::SetTimer()
{
	if (!GS)return;

	float Timer = 0;
	FText text = FText ();
	int32 Text_ID = 0;

	switch (GS->GetHASGameState())
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
		text = Text_WitchHide;
		Timer = GS->HideTimer;
		Text_ID = 19;
		break;
	}
	case  EHASGameState::SEEK:
	{
		text = Text_WitchSeek;
		Timer = GS->GameTimer;
		Text_ID = 20;
		break;
	}
	default:
		break;
	}

//	TextLabel_Time->SetText ( text );
	TextLabel_Time->Text_ID = Text_ID;
	TextLabel_Time->UpdateTextFromTag ();
	int32 min = Timer / 60;
	int32 sec = (int32)Timer % 60;

	FString str = FString::Printf(TEXT("%02d : %02d"), min, sec);
	Text_Time->SetText(FText::FromString(str));
}

void UInGameWitchUI::SetTransform ()
{
	if (!OwnerWitch) return;
} 

void UInGameWitchUI::DieEvent ( const FString& HunterNickName )
{
	bDie = true;
	Canvas_Die->SetVisibility ( ESlateVisibility::Visible );
	FText DieInfo = FText::FromString ( FString::Printf ( TEXT ( "%s에게 잡혔습니다" ) , *HunterNickName ) );
	Text_DieInfo->SetText ( DieInfo );

	FTimerHandle DieTimer;
	GetWorld ()->GetTimerManager ().SetTimer ( DieTimer , FTimerDelegate::CreateLambda ( [&] 
		{
			Canvas_Die->SetVisibility ( ESlateVisibility::Hidden );
		} ) , 3.0f , false );

}

