// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WitchIt.h"
#include "HASPlayerState.generated.h"

/**
 * 
 */


UCLASS()
class WITCHIT_API AHASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
	AHASPlayerState ();

public:
	virtual void BeginPlay () override;
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps )  const override;
public:

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_SetNickName)
	FString PlayerNickName;

	//UPROPERTY( Replicated)
	//1 == 헌터 . 2 == 위치
	UPROPERTY( ReplicatedUsing = OnRep_ChangeCharacterType )
	uint8 CharacterType = 0;
	
	UPROPERTY( Replicated )
	int32 SkillA = 0;

	UPROPERTY( Replicated )
	int32 SkillB = 1;

private:
	UFUNCTION()
	void OnRep_ChangeCharacterType ();

	UFUNCTION()
	void OnRep_SetNickName ();
public:
	//결과 화면 용 기록
	//서버에만 남겨둘지? 고민해봐야함

	//이동거리
	float MovementDistance = 0.f;
	//데미지
	float Damage = 0.0f;
	//스킬 사용 횟수
	int32 UseSkillCount = 0;
	//막타
	int32 LastKill = 0;
	//닭 스킬로 찾은 마녀 숫자
	int32 ChickFindCount = 0;
	//확인한 소품 갯수
	int32 CheckOfProps = 0;




};
