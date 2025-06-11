// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/UI/SelectTeamUI.h"
#include "KJW/Actor/BaseCharacter.h"
#include "KJW/GameMode/HideAndSeekGameMode.h"
#include "KJW/GameMode/HASGameState.h"
#include "KJW/GameMode/HASPlayerState.h"
#include "KJW/Actor/HASPlayerController.h"
#include "KJW/ActorComponent/SkillComponent.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/Textblock.h"
#include "Components/VerticalBox.h"

#include "KJW/Object/SkillBase.h"
#include "KJW/UI/PlayerBox.h"
#include "WitchIt.h"
#include "OnlineSubsystemTypes.h" 
#include "KJW/UI/MenuUI.h"

void USelectTeamUI::NativeConstruct ()
{
	Super::NativeConstruct ();

	Btn_Witch->OnClicked.AddDynamic ( this , &USelectTeamUI::SelectWitch );
	Btn_Hunter->OnClicked.AddDynamic ( this , &USelectTeamUI::SelectHunter );

	Btn_ChangeTeam->OnClicked.AddDynamic ( this , &USelectTeamUI::BackToSelect );
	Btn_Ready->OnClicked.AddDynamic ( this , &USelectTeamUI::Ready );
	Button_Menu->OnClicked.AddDynamic ( this , &USelectTeamUI::OpenMenu );


	WidgetSwitcher_WitchIt->SetActiveWidgetIndex ( 0 );
	
	VerticalBox_Team->ClearChildren ();

	GS = GetWorld ()->GetGameState<AHASGameState> ();
	if (GS)
	{
		GS->OnPlayerUpdate.AddUObject ( this , &USelectTeamUI::SetPlayerState );
		GS->OnPlayerUpdate.AddUObject ( this , &USelectTeamUI::ShowPlayerInfo );

		SetPlayerState ();
	}


}

void USelectTeamUI::SetPlayerNickName ( const FString& PlayerNickName )
{
	Text_PlayerName->SetText ( FText::FromString ( PlayerNickName ) );
}

void USelectTeamUI::SelectWitch ()
{

	bWitch = true;
	ChangeCharacterType ( EHASCharacterType::WITCH );
	ChangeTeamUI();
}

void USelectTeamUI::SelectHunter ()
{
	bWitch = false;
	ChangeCharacterType ( EHASCharacterType::HUNTER );
	ChangeTeamUI();
}

void USelectTeamUI::BackToSelect ()
{
	ChangeCharacterType ( EHASCharacterType::NONE);
	WidgetSwitcher_WitchIt->SetActiveWidgetIndex ( 0 );
}

void USelectTeamUI::Ready ()
{
	if (GS)
	{
		TSubclassOf<ABaseCharacter> SpawnClass = bWitch ? GS->WitchClass : GS->HunterClass;
		ChangeCharacter ( SpawnClass );


		//GS->StartState();
	}
	
}

void USelectTeamUI::OpenMenu ()
{
	WBP_Menu->SetVisibility ( ESlateVisibility::Visible );
}

void USelectTeamUI::ChangeCharacter ( TSubclassOf<class ABaseCharacter> CharacterClass )
{
	//FActorSpawnParameters Param;
	//Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//ABaseCharacter* BaseCharacter = GetWorld ()->SpawnActor<ABaseCharacter> ( CharacterClass , FVector () , FRotator () , Param );
	//
	
	AHASPlayerController* pc = GetOwningPlayer<AHASPlayerController>();
	////기존 pawn 제거
	auto player = pc->GetPawn ();

	if (pc)
	{
		FCharacterInfo CharacterInfo;
		CharacterInfo.bWitch = bWitch;
		CharacterInfo.CharacterClass = CharacterClass;
		CharacterInfo.SkillA_Index = 0;
		CharacterInfo.SkillB_Index = 1;

		pc->ServerRPC_RequestPossess ( CharacterInfo );
		
		// Close UI locally
		this->RemoveFromParent ();
		pc->SetShowMouseCursor ( false );
		pc->SetInputMode ( FInputModeGameOnly () );
	}
	
}


void USelectTeamUI::ChangeTeamUI()
{
	if (GS)
	{
		if (GS->GetHASGameState () == EHASGameState::READY)
		{
			Btn_Ready->SetVisibility ( ESlateVisibility::Visible );
		}
		else
		{
			Btn_Ready->SetVisibility ( ESlateVisibility::Hidden);
		}
	}

	WidgetSwitcher_WitchIt->SetActiveWidgetIndex(1);

	Img_TeamBanner->SetBrushFromTexture(bWitch ? Img_WitchBanner : Img_HunterBanner);
	TextBlock_Skill->SetColorAndOpacity(FSlateColor(bWitch ? Color_Witch : Color_Hunter));

	
	USkillBase* DefaultSkill = (bWitch ? WitchSkills[0]  : HunterSkills[0]) ->GetDefaultObject<USkillBase>();
	Img_SkillA->SetBrushFromTexture(DefaultSkill->SkillIcon);

	DefaultSkill = (bWitch ? WitchSkills[1] : HunterSkills[1])->GetDefaultObject<USkillBase>();

	Img_SkillB->SetBrushFromTexture(DefaultSkill->SkillIcon);

	ShowPlayerInfo ();
	//auto PlayerBox2 = CreateWidget ( GetWorld () , PlayerBoxClass );
	//auto PlayerBox1 = CreateWidget ( GetWorld () , PlayerBoxClass );
	//VerticalBox_Team->AddChild ( PlayerBox1 );
	//VerticalBox_Team->AddChild ( PlayerBox2 );
}

void USelectTeamUI::ShowPlayerInfo ()
{

	Text_TopHunterPlayer->SetText ( FText::AsNumber ( GS->PlayerHunter ) );
	Text_TopWitchPlayer->SetText ( FText::AsNumber ( GS->PlayerWitch ) );

	int32 PlayerCount = bWitch ?  GS->PlayerWitch: GS->PlayerHunter;

	OffPlayerBoxes ();
	
	TArray<AHASPlayerState*> PlayerStates;
	for (APlayerState* PlayerState : GS->PlayerArray)
	{
		AHASPlayerState* PS = Cast<AHASPlayerState> ( PlayerState );
		
		if (PS && PS->CharacterType == (bWitch ? 2 : 1))
		{
			PlayerStates.Add ( PS );
		}
	}




	for (int32 i = 0; i < PlayerStates.Num(); ++i)
	{
		AHASPlayerState* PS = PlayerStates[i];
	
		ShowPlayerBox ()->SetPlayerBox ( PS );
	
	}

}

UPlayerBox* USelectTeamUI::ShowPlayerBox ( )
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

	VerticalBox_Team->RemoveChild ( box );
	
	if (!box)
	{
		box = CreateWidget<UPlayerBox> ( GetWorld () , PlayerBoxClass );
		
		PlayerInfoBoxes.Add ( box );
	}

	VerticalBox_Team->AddChild ( box );

	box->SetVisibility ( ESlateVisibility::Visible );

	return box;
}

void USelectTeamUI::OffPlayerBoxes ()
{
	for (int32 i = 0; i < PlayerInfoBoxes.Num (); ++i)
		PlayerInfoBoxes[i]->SetVisibility ( ESlateVisibility::Collapsed );
}

void USelectTeamUI::SetPlayerState ()
{
	PRINTLOG ( TEXT ( "SetPlayerState" ) );

	Text_AllPlayer->SetText ( FText::AsNumber ( GS->AllPlayer ));
	Text_HunterPlayer->SetText ( FText::AsNumber ( GS->PlayerHunter ) );
	Text_WitchPlayer->SetText ( FText::AsNumber ( GS->PlayerWitch ) );

}

void USelectTeamUI::ChangeCharacterType ( EHASCharacterType Type )
{
	AHASPlayerController* pc = GetOwningPlayer<AHASPlayerController> ();
	if (pc)
	{
		pc->ServerRPC_ChangeCharacterType ( (uint8)Type );
	}
}


