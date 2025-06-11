// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/Object/SkillBase.h"
#include "SkillLuaq.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API USkillLuaq : public USkillBase
{
	GENERATED_BODY()
	

public:
	//스킬을 사용할수 있는지
	virtual bool IsStart () override;
	
	//스킬 자원 소모 함수
	virtual void UseResource () override;
	
	//스킬 게임 시 초기화 함수
	virtual void SkillInit () override;
	//스킬 동작 시작 함수
	virtual void StartSkill () override;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float readmana = 0.0f; 

};
