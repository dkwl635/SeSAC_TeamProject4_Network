// Fill out your copyright notice in the Description page of Project Settings.


#include "SSA/Skill/SkillFly.h"
#include "../ActorComponent/StateComponent.h"
#include "../Character/Witch.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

bool USkillFly::IsStart ()
{
	float mana = SkillOwner->StateComp->GetCurMana ();

	return mana > readmana;
}

void USkillFly::StartSkill ()
{
	//SkillOwner->StateComp->ConsumeMana ( readmana );

	auto Owner = Cast<AWitch> ( SkillOwner );
	Owner->StartFly();
}

void USkillFly::CompletedSkill ()
{
	//SkillOwner->StateComp->RegenMana( regenman );

	auto Owner = Cast<AWitch> ( SkillOwner );
	Owner->StopFly();
}
