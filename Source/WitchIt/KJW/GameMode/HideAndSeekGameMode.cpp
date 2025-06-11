// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/GameMode/HideAndSeekGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "KJW/Actor/Hunter.h"
#include "KJW/Actor/HASPlayerController.h"

#include "SSA/Character/Witch.h"
#include "SSA/Actor/ItemBase.h"
#include "SSA/GameInstance/NetGameInstance.h"

void AHideAndSeekGameMode::BeginPlay ()
{
	Super::BeginPlay ();	
	
	NetGameIns = GetGameInstance<UNetGameInstance> ();


	SetSessionGameState ( 0 );
}

void AHideAndSeekGameMode::ReStartGame ()
{
	if (bMoveLevel) return;
	bMoveLevel = true;

	UWorld* World = GetWorld ();
	if (!World) return;

	//UGameplayStatics::OpenLevel ( this , HASLevel );
	///Script/Engine.World'/Game/KJW/Level/HideAndSeekLevel.HideAndSeekLevel'

	// 약간의 시간 주는 것도 방법
	FTimerHandle TimerHandle;
	GetWorld ()->GetTimerManager ().SetTimer ( TimerHandle , FTimerDelegate::CreateLambda (
		[this]()
		{
			FString map = FString::Printf ( TEXT ( "%s?listen" ) , *HASLevel.ToString () );
			//GetWorld ()->ServerTravel ( map );
			GetWorld ()->ServerTravel ( TEXT ( "/Game/KJW/Level/HideAndSeekLevel?listen" ) );
		}
	) , 1.0f , false );
	

}

void AHideAndSeekGameMode::SetSessionGameState ( int32 Index )
{
	if (!NetGameIns) return;

	FString mySessionName = NetGameIns->mySessionName;
	FOnlineSessionSettings* settings = NetGameIns->sessionInterface->GetSessionSettings ( FName ( *mySessionName ) );
	if (settings)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "세션을 찾았습니다: %s" ) , *mySessionName );


		int32 currentValue = -1;
		settings->Get ( FName ( "GAME_STATE" ) , currentValue );

		settings->Set ( FName ( "GAME_STATE" ) , Index , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	}
	else
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "세션을 찾을 수 없습니다: %s" ) , *mySessionName );

	}
}

void AHideAndSeekGameMode::PostLogin ( APlayerController* NewPlayer )
{
	Super::PostLogin ( NewPlayer );

	//if (AGameStateBase* MyGameState = GetGameState<AGameStateBase>())

	// 플레이어가 들어왔을 때 처리
	++AllPlayer;
	UE_LOG ( LogTemp , Log , TEXT ( "플레이어 입장: %s" ) , *NewPlayer->GetName () );
	OnPlayerStateUpdate.Broadcast ();


}

void AHideAndSeekGameMode::Logout ( AController* Exiting )
{

	//if (AGameStateBase* MyGameState = GetGameState<AGameStateBase>())
	--AllPlayer;
	// 플레이어가 들어왔을 때 처리
	UE_LOG ( LogTemp , Log , TEXT ( "플레이어 퇴장: %s" ) , *Exiting->GetName () );
	OnPlayerStateUpdate.Broadcast ();

	Super::Logout ( Exiting );
}
