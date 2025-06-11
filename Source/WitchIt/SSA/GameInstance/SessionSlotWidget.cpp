// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSlotWidget.h"
#include "NetGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionSlotWidget::Set ( const FSessionInfo& sessionInfo )
{
	txt_roomName->SetText ( FText::FromString ( sessionInfo.roomName ) );
	txt_hostName->SetText ( FText::FromString ( sessionInfo.hostName ) );
	txt_playerCount->SetText ( FText::FromString ( sessionInfo.playerCount ) );
	txt_pingSpeed->SetText ( FText::FromString ( FString::Printf ( TEXT ( "%dms" ) , sessionInfo.pingSpeed ) ) );

	FString str;
	if (sessionInfo.gameState == 0)
	{
		str = TEXT ( "대기중" );
	}
	else
	{
		str = TEXT ( "게임중" );
	}

	txt_GameState->SetText ( FText::FromString ( str ));

	SessionNumber = sessionInfo.index; 
}

void USessionSlotWidget::NativeConstruct ()
{
	Super::NativeConstruct ();
	btn_join->OnClicked.AddDynamic ( this , &USessionSlotWidget::JoinSession );
}

void USessionSlotWidget::JoinSession ()
{
	auto gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	if (gi != nullptr)
	{
		gi->JoinSelectedSession ( SessionNumber );
	}
}
