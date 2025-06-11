// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "KJW/Data/SkillData.h"
#include "KJW/Actor/BaseCharacter.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS( Blueprintable )
class WITCHIT_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	//스킬을 사용할수 있는지
	virtual bool IsStart ();
	//스킬 게임 시 초기화 함수
	virtual void SkillInit ();
	//스킬 자원 소모 함수
	virtual void UseResource();
	//스킬 동작 시작 함수
	virtual void StartSkill ();
	//스킬 동작 시 돌아가는 Tick 함수
	virtual void SkillTick ( float DeltaTime );
	//스킬 키 입력 트리거
	virtual void TriggerSkill ( );
	//스킬 키 입력 종료
	virtual void CompletedSkill ( );

public:
	//현제 가지고 있는 주인 객체
	UPROPERTY (VisibleAnywhere )
	ABaseCharacter* SkillOwner;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	UTexture2D* SkillIcon;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	float CoolTime = 5.0f;

	UPROPERTY ( VisibleAnywhere )
	float CoolTimer = 0.0f;

	
	
	//UPROPERTY ( EditDefaultsOnly , Category = "Skill")
	////스킬 정보
	//FDataTableRowHandle SkillDataHandle;

	//UPROPERTY ( VisibleAnywhere  , Category = "Skill" )
	//FSkillData SkillData;

};
