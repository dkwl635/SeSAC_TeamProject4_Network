// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Skill/SkillChicken.h"
#include "KJW/Actor/Chicken.h"
#include "KJW/Actor/Hunter.h"

#include "WitchIt.h"

bool USkillChicken::IsStart ()
{
	//쿨타임 체크
	bool IsCool =  Super::IsStart ();
	
	return IsCool;
}

void USkillChicken::UseResource ()
{
	//쿨타임 적용
	Super::UseResource ();
}

void USkillChicken::SkillInit ()
{
}

void USkillChicken::StartSkill ()
{
	//전방으로 닭 소환
	UE_LOG ( LogTemp , Warning , TEXT ( __FUNCTION__ ) );
	AHunter* Hunter = Cast<AHunter> ( SkillOwner );
	//오른손에서 나가게
	FVector SpawnPos = Hunter->GetRightHandLocation ();
	FVector End = Hunter->GetTraceFromView ();

	//무조건 스폰되게
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//오른손에거 목적지 방향 구하기
	FVector Dir = End - SpawnPos;
	FRotator SpawnRot = Dir.Rotation ();

	AChicken* Chicken = GetWorld ()->SpawnActor<AChicken> ( ChickenBP , SpawnPos , SpawnRot , SpawnParam );
	
}


