// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HASPlayerController.generated.h"

/**
 * 
 */

USTRUCT ( BlueprintType )
struct FCharacterInfo
{
	GENERATED_BODY ()

public:
	UPROPERTY()
	bool bWitch = false;
	UPROPERTY()
	TSubclassOf<class ABaseCharacter> CharacterClass;
	UPROPERTY()
	int32 SkillA_Index = 0;
	UPROPERTY()
	int32 SkillB_Index = 1;

	inline  FString ToString () const
	{
		return FString::Printf ( TEXT ( "[%s] : SkillA : %d , SkillB : %d" ) ,bWitch ? TEXT ( "Witch" ) : TEXT ( "Hunter" ) ,SkillA_Index ,SkillB_Index );
	}

};

UCLASS()
class WITCHIT_API AHASPlayerController : public APlayerController
{
	GENERATED_BODY()
	




public:
	virtual void BeginPlay () override;
	void OpenSelectTeamUI ();
	void OpenEndGameResultUI ();
	void SetNickName ();
	void SetNickNameUI ( const FString& PlayerNickName );
public:
	//컨트롤 캐릭터 변경 요청
	UFUNCTION ( Server , Reliable )
	void ServerRPC_RequestPossess ( const FCharacterInfo CharacterInfo );
	//캐릭터 타입 변경 서버에 호출
	UFUNCTION(Server , Reliable)
	void ServerRPC_ChangeCharacterType ( uint8 Type );

	UFUNCTION ( Server , Reliable )
	void ServerRPC_SetNickName (const FString& PlayerNickName);

	UFUNCTION (Client , Reliable )
	void ClientRPC_SetNickName ();

	


protected :
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USelectTeamUI> SelectTeamUIClass;

	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class UUserWidget> EndGameResultUIClass;

	UPROPERTY()
	class USelectTeamUI* SelectTeamUI;

	UPROPERTY()
	class UUserWidget* EndGameResultUI;

	

};
