// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Skill/SkillLuaq.h"
#include "../ActorComponent/StateComponent.h"
#include "../Character/Witch.h"

bool USkillLuaq::IsStart ()
{
	float mana = SkillOwner->StateComp->GetCurMana();
	
	return mana > readmana;
}

void USkillLuaq::UseResource ()
{
	SkillOwner->StateComp->ConsumeMana ( readmana );
}

void USkillLuaq::SkillInit ()
{

}

void USkillLuaq::StartSkill ()
{
	auto Owner = Cast<AWitch>(SkillOwner);
	Owner->SkillLuaq();
}
