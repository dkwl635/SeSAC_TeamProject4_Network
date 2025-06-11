// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/GameInstance/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "NetGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "SessionSlotWidget.h"
#include "Components/ScrollBox.h"
#include "WitchIt.h"

void ULoginWidget::NativeConstruct ()
{
	Super::NativeConstruct ();
	
	gi = Cast<UNetGameInstance> ( GetWorld ()->GetGameInstance () );
	gi->onSearchCompleted.AddDynamic ( this , &ULoginWidget::AddSlotWidget );
	gi->onSearchState.AddDynamic ( this , &ULoginWidget::OnChangeButtonEnble );

	btn_createRoom->OnClicked.AddDynamic ( this , &ULoginWidget::CreateRoom );
	slider_playerCount->OnValueChanged.AddDynamic ( this , &ULoginWidget::OnValueChanged );

	Btn_CreateSession->OnClicked.AddDynamic ( this , &ULoginWidget::SwitchCreatePanel );
	Btn_FindSession->OnClicked.AddDynamic ( this , &ULoginWidget::SwitchFindPanel);

	btn_back->OnClicked.AddDynamic   ( this , &ULoginWidget::BackToMain );
	btn_back_1->OnClicked.AddDynamic ( this , &ULoginWidget::BackToMain );

	btn_find->OnClicked.AddDynamic ( this , &ULoginWidget::OnClickedFindSession );

}

void ULoginWidget::CreateRoom ()
{
	if (gi && edit_roomName->GetText ().IsEmpty () == false)
	{
		FString roomName = edit_roomName->GetText ().ToString ();
		int32 playerCount = slider_playerCount->GetValue ();
		gi->CreateMySession ( roomName , playerCount );
	}
}

void ULoginWidget::OnValueChanged ( float Value )
{
	txt_playerCount->SetText ( FText::AsNumber ( Value ) );
}

void ULoginWidget::SwitchCreatePanel ()
{
	if (edit_userName->GetText ().IsEmpty () == false)
	{
		gi->mySessionName = edit_userName->GetText ().ToString ();
	}

	WidgetSwitcher->SetActiveWidgetIndex ( 1 );
}

void ULoginWidget::SwitchFindPanel ()
{
	if (edit_userName->GetText ().IsEmpty () == false)
	{
		gi->mySessionName = edit_userName->GetText ().ToString ();
	}

	WidgetSwitcher->SetActiveWidgetIndex ( 2 );

	OnClickedFindSession ();
}

void ULoginWidget::BackToMain ()
{
	WidgetSwitcher->SetActiveWidgetIndex ( 0 );
}

void ULoginWidget::AddSlotWidget ( const struct FSessionInfo& sessionInfo )
{
	auto slot = CreateWidget<USessionSlotWidget> ( this , sessionInfoWidget );
	slot->Set ( sessionInfo );

	scroll_roomList->AddChild ( slot );
}

void ULoginWidget::OnClickedFindSession ()
{
	PRINTLOG ( TEXT ( "OnClickedFindSession" ) );
	scroll_roomList->ClearChildren ();
	if (gi != nullptr)
	{
		PRINTLOG ( TEXT ( "OnClickedFindSession1" ) );

		gi->FindOtherSession ();
	}
}

void ULoginWidget::OnChangeButtonEnble ( bool bIsSearching )
{
	btn_find->SetIsEnabled ( !bIsSearching );

	if (bIsSearching == true)
	{
		// 검색중 보이도록 처리
		txt_findingMsg->SetVisibility ( ESlateVisibility::Visible );
	}
	else
	{
		// 검색 중 사라지도록 처리
		txt_findingMsg->SetVisibility ( ESlateVisibility::Hidden );
	}
}
