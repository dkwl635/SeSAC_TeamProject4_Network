// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/Skill/SkillVacuumTrap.h"
#include "KJW/Actor/Hunter.h"
#include "KJW/Actor/VacuumTrap.h"

bool USkillVacuumTrap::IsStart ()
{
	//쿨타임 체크
	bool IsCool = Super::IsStart ();

	return IsCool;
}

void USkillVacuumTrap::SkillInit ()
{

}

void USkillVacuumTrap::StartSkill ()
{
	
	UE_LOG ( LogTemp , Warning , TEXT ( __FUNCTION__ ) );

	//쿨타임 적용
	Super::StartSkill ();

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

	AVacuumTrap* VacuumTrap = GetWorld ()->SpawnActor<AVacuumTrap> ( VacuumTrapBP , SpawnPos , SpawnRot , SpawnParam );

}
