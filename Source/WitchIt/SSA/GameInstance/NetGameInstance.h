// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
//#include "OnlineSessionInterface.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY ()

	UPROPERTY(BlueprintReadOnly)
	FString roomName; 

	UPROPERTY(BlueprintReadOnly)
	FString hostName; 

	UPROPERTY(BlueprintReadOnly)
	FString playerCount;

	UPROPERTY(BlueprintReadOnly)
	int32 pingSpeed; 

	UPROPERTY(BlueprintReadOnly)
	int32 index; 

	UPROPERTY ( BlueprintReadOnly )
	int32 gameState;

	inline FString ToString ()
	{
		return FString::Printf ( TEXT ( "[%d] %s : %s - %s, %dms : GameState : %d" ) , index , *roomName , *hostName , *playerCount , pingSpeed , gameState );
	}
};

// 세션검색 끝났을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam ( FSearchSignature , const FSessionInfo& , sessionInfo );

// 세션검색 상태 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam ( FSearchStateSignature , bool , bIsSearching );

/**
 * 
 */
UCLASS()
class WITCHIT_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init () override;

public:
	IOnlineSessionPtr sessionInterface; 

	void CreateMySession ( FString roomName , int32 playerCount );

	// 세션 호스트 이름
	FString mySessionName = "Oreo";

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	TSharedPtr<FOnlineSessionSearch> sessionSearch; 

	void FindOtherSession ();

	void OnFindSessionsComplete ( bool bWasSuccessful );

	// 방찾기 완료 콜백을 등록할 델리게이트
	FSearchSignature onSearchCompleted;
	
	// 방찾기상태 콜백 델리게이트
	FSearchStateSignature onSearchState;

	// 다국어 코딩
	FString StringBase64Encode ( const FString& str );

	FString StringBase64Decode ( const FString& str );

	void JoinSelectedSession ( int32 index );

	void OnJoinSessionCompleted ( FName sessionName , EOnJoinSessionCompleteResult::Type result );

	
	
	//세션 나가기
	void ExitRoom();
	//세션 강제 추방 당했을때?
	UFUNCTION()
	void OnMyExitRoomComplete ( FName sessionName , bool bWasSuccessful );
	//게임 종료
	void GameExit ();
	
};
