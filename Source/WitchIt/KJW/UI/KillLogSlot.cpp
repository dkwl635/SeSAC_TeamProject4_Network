// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/KillLogSlot.h"
#include "Components/TextBlock.h"


void UKillLogSlot::SetText(FString HunterName, FString WitchName)
{
	Text_HunterNick->SetText(FText::FromString(HunterName));
	Text_WitchNick->SetText(FText::FromString(WitchName));
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&] 
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
		}), 3.0f, false);
}
