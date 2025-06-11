// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Object/SkillBase.h"



bool USkillBase::IsStart()
{
	return CoolTimer <= 0.0f;
}

void USkillBase::SkillInit()
{
	

}

void USkillBase::UseResource ()
{
	CoolTimer = CoolTime;
}

void USkillBase::StartSkill()
{
	
}


void USkillBase::SkillTick(float DeltaTime)
{
	if (CoolTimer > 0)
	{
		CoolTimer -= DeltaTime;
	}

}

void USkillBase::TriggerSkill()
{
}

void USkillBase::CompletedSkill()
{
}
