// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/Object/SkillBase.h"
#include "SkillFly.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API USkillFly : public USkillBase
{
	GENERATED_BODY()
	
	//스킬을 사용할수 있는지
	virtual bool IsStart () override;

	//스킬 동작 시작 함수
	virtual void StartSkill () override;

	//스킬 키 입력 종료
	virtual void CompletedSkill () override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float readmana = 0.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float regenman = 0.0f;
};
