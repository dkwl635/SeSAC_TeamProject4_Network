// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HASGameState.generated.h"

/**
 *
 */
UENUM ( BlueprintType )
enum class EHASGameState : uint8
{ 
	NONE,
	READY,
	HIDE,
	SEEK,
	END,
	MAPMOVE,
};



UCLASS ()
class WITCHIT_API AHASGameState : public AGameStateBase
{
	GENERATED_BODY ()

public:
	friend class AHideAndSeekGameMode;

public: 
	AHASGameState ();
	virtual void BeginPlay ()override;
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps )  const override;
	virtual void Tick ( float DeltaSeconds ) override;

	//여기 함수 호출은 거의 서버에서 호출될 함수
	void StartState ();
	void DieWitch ( APlayerController* WitchPc );

	void SetCharacterCount ();

	//마녀 처치시 누가 죽었는지
	void DieWitchLog ( FString HunterName ,FString WitchName );

public:
	TArray<class ABaseCharacter*>& GetWitches () { return Witches; }
	TArray<class ABaseCharacter*>& GetHunters () { return Hunters; }
	TArray<class AItemBase*>& GetItems () { return Items; }
	TArray<TObjectPtr<APlayerState>>* GetPlayerStates ();
	bool IsWinWitch () { return bWitchWin; }

	EHASGameState GetHASGameState () { return HASGameState; }

	FVector GetSpawnPoint ( bool bWitch );
public:
	UPROPERTY ( ReplicatedUsing = OnRep_UpdatePlayer )
	int32 PlayerWitch = 0;
	UPROPERTY ( ReplicatedUsing = OnRep_UpdatePlayer )
	int32 PlayerHunter = 0;
	UPROPERTY ( ReplicatedUsing = OnRep_UpdatePlayer )
	int32 AllPlayer = 0;

	int32 AlliveWitch = 0;

	UPROPERTY(EditAnywhere, Replicated )
	float GameTimer = 120.0f;
	UPROPERTY ( EditAnywhere , Replicated )
	float HideTimer = 30.0f;
	UPROPERTY ( EditAnywhere , Replicated )
	float NextGameTimer = 7.0f;


	//스킬
	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	TArray<TSubclassOf<class USkillBase>> HunterSkills;

	UPROPERTY ( EditDefaultsOnly , Category = WitchIt )
	TArray<TSubclassOf<class USkillBase>> WitchSkills;

	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class ABaseCharacter> HunterClass;
	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class ABaseCharacter> WitchClass;

	UPROPERTY ( EditDefaultsOnly  )
	FText Text_WinHunter;

	UPROPERTY ( EditDefaultsOnly  )
	FText Text_WinWitch;

	//서버에서 사용되는 함수 , 통신용 함수 X
	void Server_ChangeGameState ( EHASGameState  NewState );



	//플레이어 업데이트에 따른 갱신 필요 델리게이트
	FSimpleMulticastDelegate OnPlayerUpdate;
	//캐릭터 타입 변경완료시
	//FSimpleMulticastDelegate OnPlayerCharacterUpdate;
	
	UPROPERTY()
	class UKillLogUI* KillLogUI;

public:
	void SetSuccessCharacter ();
	UFUNCTION ( Server , Reliable )
	void SetSelectCharacter ( bool bWitch );

private:
	UPROPERTY()
	class AHideAndSeekGameMode* GM;

	UPROPERTY( ReplicatedUsing = OnRep_UpdateState )
	EHASGameState HASGameState = EHASGameState::NONE;

	UPROPERTY ( VisibleAnywhere )
	TArray<class ABaseCharacter*> Witches;
	UPROPERTY ( VisibleAnywhere )
	TArray<class ABaseCharacter*> Hunters;
	UPROPERTY ( VisibleAnywhere )
	TArray<class AItemBase*> Items;

	
	//캐릭터 생성이 완료된 플레이어
	int32 CreateSuccessPlayer = 0;


	UPROPERTY(Replicated)
	bool bWitchWin = true;
	bool IsStartGame = false;

	UPROPERTY()
	class ASpawnPoint* WitchSpawner;
	UPROPERTY()
	class ASpawnPoint* HunterSpawner;


private:
	//필드 케릭터 찾기
	void SetHunters ();
	void SetWitches ();
	void SetItems ();
	void EndGame ();
	void MoveLevel ();
	void SetSpawner ();
	void DestroyWall ();

	//플레이어들 정보 갱신
	void OnPlayerStateUpdate ();

	UFUNCTION ()
	void OnRep_UpdatePlayer ();

	UFUNCTION ()
	void OnRep_UpdateState ();

	UFUNCTION(NetMulticast , Reliable )
	void MulticastRPC_DieWitchLog (const FString& HunterName , const FString& WitchName );

	void CloseSession ();
};
