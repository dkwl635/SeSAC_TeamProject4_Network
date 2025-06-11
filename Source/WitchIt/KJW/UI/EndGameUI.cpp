// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/UI/EndGameUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "KJW/Widget/KTextBlock.h"
#include "GameFramework/PlayerController.h"

#include "KJW/Actor/BaseCharacter.h"
#include "KJW/GameMode/HideAndSeekGameMode.h"
#include "KJW/GameMode/HASGameState.h"
#include "KJW//UI/MenuUI.h"

void UEndGameUI::NativeConstruct ()
{
	Super::NativeConstruct ();


	Button_Retry->OnClicked.AddDynamic ( this , &UEndGameUI::RetryGame );
	Button_Menu->OnClicked.AddDynamic ( this , &UEndGameUI::OpenMenu );

	
	GS = GetWorld ()->GetGameState<AHASGameState> ();

	int32 Text_ID = 0;
	if (GS)
	{
		Text_ID = (GS->IsWinWitch()) ? 22 : 21;
		Text_Result->Text_ID = Text_ID;
		Text_Result->UpdateTextFromTag ();
		//Text_Result->SetText ( GS->IsWinWitch() ? GS->Text_WinWitch : GS->Text_WinHunter );
	}

	FTimerHandle timerHandle;
	GetWorld ()->GetTimerManager ().SetTimer ( timerHandle , FTimerDelegate::CreateLambda ( [&] {
		OpenResultUI ();
		} ) , 2.0f , false );

	APlayerController* PC = GetOwningPlayer ();
	if (PC && !PC->HasAuthority ())
	{
		Button_Retry->SetVisibility ( ESlateVisibility::Hidden );
	}
}

void UEndGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
	SetTimer();
}

void UEndGameUI::RetryGame ()
{
	Button_Retry->SetVisibility ( ESlateVisibility::Hidden );
	
	if (!GS) return;
	GS->Server_ChangeGameState ( EHASGameState::MAPMOVE );
}

void UEndGameUI::OpenResultUI ()
{
	Canvas_Result->SetVisibility ( ESlateVisibility::Visible );

	auto pc = GetOwningPlayer ();
	if (pc)
	{
		pc->FlushPressedKeys ();
		pc->SetInputMode ( FInputModeUIOnly() );
		pc->SetShowMouseCursor(true);
		
		ABaseCharacter* pawn = pc->GetPawn<ABaseCharacter>();
		if (pawn)
		{
			pawn->EndGame();
		}
	}
	
}

void UEndGameUI::SetTimer()
{
	if (GS->IsValidLowLevel())
	{
		int32 Text_ID = 0;
		if (GS->GetHASGameState () == EHASGameState::MAPMOVE)
		{
			Text_NextTimer->SetText ( FText::FromString ( "" ) );
			Text_ID = 23;
			Button_Retry->SetVisibility ( ESlateVisibility::Hidden );
			//Text_LevelMove->SetText ( FText::FromString ( TEXT ( "맵 이동 중..." ) ) );
		}
		else
		{
			int32 timer = GS->NextGameTimer;
			Text_ID = 24;
			Text_NextTimer->SetText ( FText::AsNumber ( timer ) );
		}
		
		Text_LevelMove->Text_ID = Text_ID;
		Text_LevelMove->UpdateTextFromTag ();
	
	}
}

void UEndGameUI::OpenMenu ()
{
	WBP_Menu->SetVisibility ( ESlateVisibility::Visible );
}
