// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/Object/UIWorldSubsystem.h"
#include "KJW/Object/HASTextDataAsset.h"
#include "KJW/Object/HASTextGameInstanceSubsystem.h"


void UUIWorldSubsystem::Initialize ( FSubsystemCollectionBase& Collection )
{
	Super::Initialize ( Collection );
	OnChangeLanguage.Clear ();
}

void UUIWorldSubsystem::Deinitialize ()
{
	OnChangeLanguage.Clear ();
	Super::Deinitialize ();
}

void UUIWorldSubsystem::ChangeLanguage ( int32 LanguageIndex )
{
	//UHASTextDataAsset::GetInstance ()->Language = LanguageIndex;

	UHASTextGameInstanceSubsystem* subsystem = GetWorld ()->GetGameInstance()->GetSubsystem<UHASTextGameInstanceSubsystem> ();
	if (subsystem)
	{
		subsystem->Language = LanguageIndex;
	}

	//KText로 연결된 Text 새로고침
	OnChangeLanguage.Broadcast ();
}
