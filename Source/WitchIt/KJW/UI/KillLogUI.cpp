// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/UI/KillLogUI.h"
#include "KJW/UI/KillLogSlot.h"
#include "KJW/GameMode/HASGameState.h"
#include "Components/VerticalBox.h"

void UKillLogUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	AHASGameState* GS = GetWorld ()->GetGameState<AHASGameState> ();
	if (GS)
	{
		GS->KillLogUI = this;
	}
}

void UKillLogUI::ShowKillLog ( FString HunterName , FString WitchName )
{

	UKillLogSlot* slot = GetLogSlot ();

	Vertical_KillLog->RemoveChild ( slot );

	slot->SetVisibility ( ESlateVisibility::Visible );
	slot->SetText ( HunterName , WitchName );

	Vertical_KillLog->InsertChildAt ( 0 , slot );

}

UKillLogSlot* UKillLogUI::GetLogSlot ()
{
	UKillLogSlot* slot = nullptr;

	for (UKillLogSlot* playerBox : KillLogSlots)
	{
		if (playerBox->GetVisibility () != ESlateVisibility::Visible)
		{
			slot = playerBox;
			continue;
		}
	}

	if (!slot)
	{
		slot = CreateWidget<UKillLogSlot> ( GetWorld () , KillLogSlotClass );

		KillLogSlots.Add ( slot );
	}

	return slot;
}
