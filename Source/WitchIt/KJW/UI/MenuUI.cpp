// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/MenuUI.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "KJW/Object/UIWorldSubsystem.h"
#include "SSA/GameInstance/NetGameInstance.h"



void UMenuUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	Button_Continue->OnClicked.AddDynamic ( this , &UMenuUI::Continue );
	Button_Language->OnClicked.AddDynamic ( this , &UMenuUI::Language);
	Button_Exit->OnClicked.AddDynamic ( this , &UMenuUI::Exit );

	Button_Kor->OnClicked.AddDynamic ( this , &UMenuUI::Change_Kor );
	Button_Eng->OnClicked.AddDynamic ( this , &UMenuUI::Change_Eng );
}

void UMenuUI::Continue ()
{
	this->SetVisibility ( ESlateVisibility::Collapsed );
}


void UMenuUI::Language ()
{
	Switcher->SetActiveWidgetIndex ( 1 );
}

void UMenuUI::Exit ()
{
	UE_LOG ( LogTemp , Warning , TEXT ( "Exit" ) );

	UNetGameInstance* gi = GetGameInstance<UNetGameInstance> ();
	if (gi)
	{
		gi->ExitRoom ();
	}

}

void UMenuUI::Change_Kor ()
{
	UUIWorldSubsystem* subsystem = GetWorld ()->GetSubsystem<UUIWorldSubsystem> ();
	if (subsystem)
	{
		subsystem->ChangeLanguage ( 0 );
	}

	Switcher->SetActiveWidgetIndex ( 0 );
}

void UMenuUI::Change_Eng ()
{
	UUIWorldSubsystem* subsystem = GetWorld ()->GetSubsystem<UUIWorldSubsystem> ();
	if (subsystem)
	{
		subsystem->ChangeLanguage ( 1 );
	}

	Switcher->SetActiveWidgetIndex ( 0 );
}


