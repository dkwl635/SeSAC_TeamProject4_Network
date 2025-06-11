// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/GameMode/HASPlayerState.h"
#include "Net/UnrealNetwork.h"

#include "KJW/GameMode/HASGameState.h"
#include "KJW/Actor/HASPlayerController.h"

AHASPlayerState::AHASPlayerState ()
{
	bReplicates = true;
}

void AHASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void AHASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME ( AHASPlayerState , PlayerNickName );
	DOREPLIFETIME ( AHASPlayerState , CharacterType );
	DOREPLIFETIME ( AHASPlayerState , SkillA );
	DOREPLIFETIME ( AHASPlayerState , SkillB );


}

void AHASPlayerState::OnRep_ChangeCharacterType ()
{
	//현재 각 클라이언트
	//GameState에 가서 다시 
	AHASGameState* GS = GetWorld ()->GetGameState<AHASGameState> ();
	if (GS)
	{
		GS->SetCharacterCount ();
	}
}

void AHASPlayerState::OnRep_SetNickName ()
{
	//auto a = GetOwner ();
	//
	//AHASPlayerController* PC = Cast<AHASPlayerController>(GetPlayerController ());
	//if (PC)
	//{
	//	PC->SetNickNameUI ();
	//}

}
