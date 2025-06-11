// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/GameInstance/NetGameInstance.h"
#include "Online/CoreOnlineFwd.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "Online\OnlineSessionNames.h"
#include "WitchIt.h"



void UNetGameInstance::Init ()
{
	Super::Init();

	if (auto subsys = IOnlineSubsystem::Get ())
	{
		// 서브시스템으로부터 세션 인터페이스 가져오기
		sessionInterface = subsys->GetSessionInterface ();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject ( this , &UNetGameInstance::OnCreateSessionComplete );
		
		//sessionInterface->OnCancelFindSessionsCompleteDelegates.AddUObject ( this , &UNetGameInstance::OnFindSessionsComplete );
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject ( this , &UNetGameInstance::OnFindSessionsComplete );

		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject ( this , &UNetGameInstance::OnJoinSessionCompleted );
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject ( this , &UNetGameInstance::OnMyExitRoomComplete );

		mySessionName.Append(FString::Printf(TEXT("_%d,%d"), FMath::Rand32(), FDateTime::Now().GetMillisecond()));
	}


	/*FTimerHandle handle;
	GetWorld ()->GetTimerManager ().SetTimer ( handle ,
	FTimerDelegate::CreateLambda ( [&]
		{
			FindOtherSession();
		}) , 2.0f , false );*/

	/*FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle,
	FTimerDelegate::CreateLambda([&]
		  {
				CreateMySession(mySessionName, 10);
		  }
	
	), 2.0f, false);*/

}

void UNetGameInstance::CreateMySession ( FString roomName , int32 playerCount )
{
	// 세션 설정 변수
	FOnlineSessionSettings sessionSettings;

	// 1. Dedicated Server 접속여부
	sessionSettings.bIsDedicated = false;

	// 2. 랜선 (로컬) 매칭을 할지 steam 매칭을 할지 여부 
	FName subsystemName = IOnlineSubsystem::Get ()->GetSubsystemName ();
	sessionSettings.bIsLANMatch = (subsystemName == "Null");

	// 3. 매칭이 온라인을 통해 노출될지 여부
	// false 이면 초대를 통해서만 입장이 가능
	// SendSessioinInviteToFriend() 함수를 통해 친구를 초대할 수 있다. 
	sessionSettings.bShouldAdvertise = true;

	// 4. 온라인 상태 (Presence) 정보를 활용할지 여부 
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 5. 게임진행중에 참여 허가할지 여부 
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bAllowJoinInProgress = true;

	// 6. 세션에 참여할 수 있는 공개 (public) 연결의 최대 허용 수 
	sessionSettings.NumPublicConnections = playerCount;

	// 7. 커스텀 롱네임 설정
	sessionSettings.Set ( FName ( "ROOM_NAME" ) ,StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );

	// 8. 호스트 네임 설정

	sessionSettings.Set ( FName ( "HOST_NAME" ) ,StringBase64Encode(mySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );

	// 9. 게임 분류
	FString GAME_NAME = FString::Printf ( TEXT ( "HAS" ) );
	sessionSettings.Set ( FName ( "GAME_NAME" ) , GAME_NAME , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	//10. 커스텀 세션 정보 추가
	sessionSettings.Set ( FName ( "GAME_STATE" ) , 0 , EOnlineDataAdvertisementType::ViaOnlineServiceAndPing );
	
	// net ID

	FUniqueNetIdPtr netID = GetWorld ()->GetFirstLocalPlayerFromController ()->GetUniqueNetIdForPlatformUser ().GetUniqueNetId ();

	
	PRINTLOG ( TEXT ( "Create Session Start : %d" ) , *mySessionName );
	sessionInterface->CreateSession ( *netID , FName ( mySessionName ) , sessionSettings );


}

void UNetGameInstance::OnCreateSessionComplete ( FName SessionName , bool bWasSuccessful )
{
	PRINTLOG ( TEXT ( "SessionName : %s, bWasSuccessful : %d" ) , *SessionName.ToString(), bWasSuccessful);

	if (bWasSuccessful == true)
	{
		///Script/Engine.World'/Game/KJW/Level/HideAndSeekLevel.HideAndSeekLevel'
		GetWorld()->ServerTravel(TEXT("/Game/KJW/Level/HideAndSeekLevel?listen")); // TEXT에 URL이 들어감 
	}
}

void UNetGameInstance::FindOtherSession ()
{
	onSearchState.Broadcast ( true );
	PRINTLOG ( TEXT ( "FindOtherSession..." ) );
	sessionSearch = MakeShareable ( new FOnlineSessionSearch () );

	// 1.세션 검색 조건 설정
	FString GAME_NAME = FString::Printf ( TEXT ( "HAS" ) );

	sessionSearch->QuerySettings.Set ( SEARCH_PRESENCE , true , EOnlineComparisonOp::Equals );
	sessionSearch->QuerySettings.Set ( FName ( "GAME_NAME" ) , GAME_NAME ,EOnlineComparisonOp::Equals);

	// 2. Lan 여부
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get ()->GetSubsystemName () == FName ( "NULL" );

	// 3. 최대 검색 세션 수 
	sessionSearch->MaxSearchResults = 10;

	// 4. 세션 검색여부
	sessionInterface->FindSessions ( 0 , sessionSearch.ToSharedRef () );
}

void UNetGameInstance::OnFindSessionsComplete ( bool bWasSuccessful )
{
	onSearchState.Broadcast ( false );
	// 찾기 실패 시 
	if (bWasSuccessful == false)
	{
		PRINTLOG( TEXT ( "Session search failed..." ) );
		return;
	}

	// 세션 검색 결과 배열
	auto results = sessionSearch->SearchResults;
	PRINTLOG ( TEXT ( "Search Result Cont : %d" ) , results.Num () );

	// 정보를 가져온다. 
	for (int i = 0; i < results.Num(); ++i)
	{
		// 정보가 유효한지 체크
		auto sr = results[i];
		if (sr.IsValid () == false)
		{
			continue;
		}

		// for Decode
		FString roomName;
		FString hostName;
		FString GameName;

		FSessionInfo sessionInfo;
		sessionInfo.index = i;

		//sr.Session.SessionSettings.Get ( FName ( "ROOM_NAME" ) , sessionInfo.roomName );
		sr.Session.SessionSettings.Get ( FName ( "ROOM_NAME" ) , roomName );

		//sr.Session.SessionSettings.Get ( FName ( "HOST_NAME" ) , sessionInfo.hostName );
		sr.Session.SessionSettings.Get ( FName ( "HOST_NAME" ) , hostName );

		sr.Session.SessionSettings.Get ( FName ( "GAME_NAME" ) , GameName );
		if (!GameName.Equals ( "HAS" ))
		{
			continue;
		}

		sessionInfo.roomName = StringBase64Decode ( roomName );
		sessionInfo.hostName = StringBase64Decode ( hostName );

		int32 GameState = -1;
		sr.Session.SessionSettings.Get ( FName ( "GAME_STATE" ) , GameState );
		sessionInfo.gameState = GameState;
		// 입장 가능한 플레이어수 

		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;

		// 현재 입장가능한 플레이어수 (최대 - 현재 입장가능한 수 )
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;

		sessionInfo.pingSpeed = sr.PingInMs;

		sessionInfo.playerCount = FString::Printf ( TEXT ( "(%d/%d)" ) , currentPlayerCount , maxPlayerCount ); 

		PRINTLOG ( TEXT ( "%s" ) , *sessionInfo.ToString () );

		onSearchCompleted.Broadcast ( sessionInfo );
	}

}

FString UNetGameInstance::StringBase64Encode ( const FString& str )
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArry<uint8> -> base64로 Encode 

	std::string utf8String = TCHAR_TO_UTF8 ( *str );
	TArray<uint8> arrayData = TArray<uint8>((uint8*)utf8String.c_str(), utf8String.length ());
	
	return FBase64::Encode ( arrayData );
}

FString UNetGameInstance::StringBase64Decode ( const FString& str )
{
	// Get 할 때 ( 서버에서 방 정보들을 받아올 때)
	// : base64로 Decode -> TArray<uint8> -> TCHAR 
	TArray<uint8> arrayData;
	FBase64::Decode ( str , arrayData );
	std::string utf8String((char*)(arrayData.GetData()), arrayData.Num());

	return UTF8_TO_TCHAR(utf8String.c_str());
}

void UNetGameInstance::JoinSelectedSession ( int32 index )
{
	auto sr = sessionSearch->SearchResults;

	sr[index].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	sr[index].Session.SessionSettings.bUsesPresence = true;


	sessionInterface->JoinSession ( 0 , FName ( mySessionName ) , sr[index]);
}

void UNetGameInstance::OnJoinSessionCompleted ( FName sessionName , EOnJoinSessionCompleteResult::Type result )
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		auto pc = GetWorld ()->GetFirstPlayerController ();

		FString url;
		sessionInterface->GetResolvedConnectString ( sessionName , url );

		PRINTLOG ( TEXT ( "Join URL : %s" ) , *url );

		if (!url.IsEmpty ())
		{
			pc->ClientTravel ( url , ETravelType::TRAVEL_Absolute );
		}
	}
	else
	{
		PRINTLOG ( TEXT ( "Join Session failed : %d" ) , result );
	}
}

void UNetGameInstance::ExitRoom ()
{
	if (sessionInterface.IsValid ())
	{
		sessionInterface->DestroySession ( FName ( *mySessionName ) );
	}
}

void UNetGameInstance::OnMyExitRoomComplete ( FName sessionName , bool bWasSuccessful )
{
	auto pc = GetWorld ()->GetFirstPlayerController ();
	///Script/Engine.World'/Game/SSA/Map/LobbyMap.LobbyMap'
	FString url = TEXT ( "/Game/SSA/Map/LobbyMap" );
	pc->ClientTravel ( url , TRAVEL_Absolute );
}

void UNetGameInstance::GameExit ()
{

}
