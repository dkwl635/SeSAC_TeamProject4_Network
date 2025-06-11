// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/Object/SkillBase.h"
#include "SkillVacuumTrap.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API USkillVacuumTrap : public USkillBase
{
	GENERATED_BODY()

public:
	//스킬을 사용할수 있는지
	virtual bool IsStart ();
	//스킬 게임 시 초기화 함수
	virtual void SkillInit ();
	//스킬 동작 시작 함수
	virtual void StartSkill ();

private:
	UPROPERTY ( EditDefaultsOnly , Category = SkillVacuumTrap )
	TSubclassOf<class AVacuumTrap> VacuumTrapBP;
};
