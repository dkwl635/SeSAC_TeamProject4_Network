// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/UI/PlayerListUI.h"
#include "KJW/GameMode/HASPlayerState.h"
#include "KJW/GameMode/HASGameState.h"
#include "KJW/UI/PlayerBox.h"

#include "Components/VerticalBox.h"

void UPlayerListUI::NativeConstruct()
{
	Super::NativeConstruct();

	GS = GetWorld()->GetGameState<AHASGameState>();
	GS->OnPlayerUpdate.AddUObject ( this , &UPlayerListUI::SetPlayersInfo );
}

void UPlayerListUI::SetPlayersInfo()
{
	HideAllBox ();

	for (APlayerState* ps : GS->PlayerArray)
	{
		AHASPlayerState* PS = Cast<AHASPlayerState> ( ps );

		UPlayerBox* box = GetPlayerBox ();
		
		if (PS->CharacterType == 2)
			VerticalBox_Witch->AddChild ( box );
		else if (PS->CharacterType == 1)
			VerticalBox_Hunter->AddChild ( box );

		box->SetPlayerBox ( PS );
		box->SetVisibility ( ESlateVisibility::Visible );
	}	

}

UPlayerBox* UPlayerListUI::GetPlayerBox ()
{
	UPlayerBox* box = nullptr;

	for (UPlayerBox* playerBox : PlayerInfoBoxes)
	{
		if (playerBox->GetVisibility () != ESlateVisibility::Visible)
		{
			box = playerBox;
			continue;
		}
	}


	if (!box)
	{
		box = CreateWidget<UPlayerBox> ( GetWorld () , PlayerBoxClass );

		PlayerInfoBoxes.Add ( box );
	}

	return box;
}

void UPlayerListUI::HideAllBox ()
{
	VerticalBox_Witch->ClearChildren ();
	VerticalBox_Hunter->ClearChildren ();

	for (int i = 0; i < PlayerInfoBoxes.Num (); ++i)
	{
		PlayerInfoBoxes[i]->SetVisibility ( ESlateVisibility::Collapsed );
	}
}
