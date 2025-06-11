// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/Actor/HASPlayerController.h"
#include "KJW/Actor/BaseCharacter.h"
#include "KJW/GameMode/HASPlayerState.h"
#include "KJW/GameMode/HASGameState.h"
#include "KJW/UI/SelectTeamUI.h"
#include "KJW/ActorComponent/SkillComponent.h"
#include "SSA/GameInstance/NetGameInstance.h"

void AHASPlayerController::BeginPlay ()
{
	Super::BeginPlay ();
	

	OpenSelectTeamUI ();
	SetNickName ();


}

void AHASPlayerController::OpenSelectTeamUI ()
{
	if (!IsLocalPlayerController ())
		return;

	if (SelectTeamUIClass)
	{
		SelectTeamUI = CreateWidget<USelectTeamUI> (this ,SelectTeamUIClass );
		SelectTeamUI->AddToViewport ();
	
		this->SetShowMouseCursor ( true );
	}


}

void AHASPlayerController::OpenEndGameResultUI ()
{
	if (!IsLocalPlayerController ())
		return;

	if (EndGameResultUIClass)
	{
		EndGameResultUI = CreateWidget<UUserWidget> ( this , EndGameResultUIClass );
		EndGameResultUI->AddToViewport ();

		//this->SetShowMouseCursor ( true );
	}

	

}

void AHASPlayerController::SetNickName ()
{
	if (!IsLocalPlayerController ()) return;
	
	FString NickFName = FString::Printf(TEXT("Player : %d" ), FMath::RandRange (1 , 999));
	
	UNetGameInstance* gi = GetGameInstance<UNetGameInstance> ();
	if (gi)
	{
		NickFName = gi->mySessionName;
	}
	
	
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState> ();
	if (PS)
	{
		PS->PlayerNickName = NickFName;
	}

	SetNickNameUI ( NickFName );
	ServerRPC_SetNickName ( NickFName );
	
}

void AHASPlayerController::SetNickNameUI ( const FString& PlayerNickName )
{
	if (SelectTeamUI)SelectTeamUI->SetPlayerNickName ( PlayerNickName );

	AHASPlayerState* PS = GetPlayerState<AHASPlayerState> ();
	if (PS)
	{
		PS->PlayerNickName = PlayerNickName;
	}
}


void AHASPlayerController::ClientRPC_SetNickName_Implementation ()
{
	//if (SelectTeamUI)SelectTeamUI->SetPlayerNickName ();
}


void AHASPlayerController::ServerRPC_SetNickName_Implementation ( const FString& PlayerNickName )
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState> ();
	if (PS)
	{
		PS->PlayerNickName = PlayerNickName;
		//ClientRPC_SetNickName ();
	}
}

void AHASPlayerController::ServerRPC_ChangeCharacterType_Implementation(uint8 Type)
{
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState>();
	if (PS)
	{
		PS->CharacterType = Type;

		AHASGameState* GS = GetWorld()->GetGameState<AHASGameState>();
		if (GS)
		{
			GS->SetCharacterCount();
		}
	}
}

void AHASPlayerController::ServerRPC_RequestPossess_Implementation ( const FCharacterInfo CharacterInfo )
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector spawnPos = FVector ( 0.0f , 0.0f , 100.0f );
	AHASGameState* GS = GetWorld ()->GetGameState<AHASGameState> ();
	if (GS)
	{
		spawnPos = GS->GetSpawnPoint ( CharacterInfo.bWitch );
	}

	ABaseCharacter* BaseCharacter = GetWorld ()->SpawnActor<ABaseCharacter> ( CharacterInfo.CharacterClass , spawnPos , FRotator () , Param );
	if (!BaseCharacter) return;

	// Destroy old pawn
	APawn* Oldpawn = GetPawn ();
	if (Oldpawn)
	{
		UnPossess ();
		Oldpawn->Destroy ();
	}

	
	UE_LOG ( LogTemp , Warning , TEXT ( "%s" ) , *(CharacterInfo.ToString ())) ;
	// Possess new pawn
	Possess ( BaseCharacter );
	BaseCharacter->SetOwner ( this );
	BaseCharacter->SetCreateInit ( CharacterInfo );

	
	AHASPlayerState* PS = GetPlayerState<AHASPlayerState> ();
	if (PS)
	{
		BaseCharacter->OwnerNickName = PS->PlayerNickName;
	}
}
