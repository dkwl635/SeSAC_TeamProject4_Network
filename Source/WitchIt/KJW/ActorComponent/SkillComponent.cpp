// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/ActorComponent/SkillComponent.h"
#include "KJW/Actor/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "WitchIt.h"
// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	if (SkillActors.Num () > 0)
		InitSkillComponent ();
	// ...

	
}

void USkillComponent::InitializeComponent ()
{
	Super::InitializeComponent ();
	SetIsReplicated ( true );
}

void USkillComponent::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );
	DOREPLIFETIME ( USkillComponent ,  bTest);
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SkillTick ( DeltaTime );	
}

void USkillComponent::StartSkillA ()
{
	//우선 스킬이 2개 일때만
	if (Skills.Num () != 2) return;
	if (!IsStartSkill ( 0 ))return;
	
	Skills[0]->UseResource ();

	
	ServerRPC_StartSkill ( 0 );
	//Skills[0]->StartSkill ();
}

void USkillComponent::TriggerSkillA ()
{
}

void USkillComponent::CompletedSkillA ()
{
	if (Skills.Num () != 2) return;

	//Skills[0]->CompletedSkill ();

	ServerRPC_CompletedSkill ( 0 );
}

void USkillComponent::StartSkillB ()
{
	//우선 스킬이 2개 일때만
	if (Skills.Num () != 2) return;
	if (!IsStartSkill ( 1 ))return;

	Skills[1]->UseResource ();
	ServerRPC_StartSkill ( 1 );
}


void USkillComponent::TriggerSkillB ()
{
}

void USkillComponent::CompletedSkillB ()
{
	if (Skills.Num () != 2) return;

	//Skills[1]->CompletedSkill();
	ServerRPC_CompletedSkill ( 1 );
}

void USkillComponent::ServerRPC_StartSkill_Implementation ( int32 Index )
{
	//Skills[Index]->StartSkill ();

	//우선 스킬이 2개 일때만
	if (Skills.Num () != 2) return;
	//if (!IsStartSkill ( Index ))return;

	//Skills[1]->StartSkill ();
	//Skills[Index]->UseResource ();
	Skills[Index]->StartSkill ();

}




void USkillComponent::ServerRPC_CompletedSkill_Implementation ( int32 Index )
{
	Skills[Index]->CompletedSkill ();
}

USkillBase* USkillComponent::GetSkill ( int32 SkillIndex )
{
	//우선 스킬이 2개 일때만
	if (Skills.Num () != 2) return nullptr;

	
	return Skills[SkillIndex];
}



void USkillComponent::InitSkillComponent ()
{
	if (SkillActors.Num () <= 0) return;

	OwnerCharacter = Cast<ABaseCharacter>(GetOwner ());

	Skills.Empty ();

	for (TSubclassOf<USkillBase> SkillClass : SkillActors)
	{
		if (SkillClass)
		{
			USkillBase* NewSkill = NewObject<USkillBase> ( this , SkillClass ); // Outer는 보통 매니저나 소유 객체
			if (NewSkill)
			{
				NewSkill->SkillOwner = OwnerCharacter;
				NewSkill->SkillInit ();       
				Skills.Add ( NewSkill );
			}
		}
	}
}

void USkillComponent::SkillTick ( float DeltaTime )
{
	for (int32 i = 0; i < Skills.Num (); ++i)
	{
		Skills[i]->SkillTick ( DeltaTime );
	}
}

bool USkillComponent::IsStartSkill ( int32 SkillIndex )
{
	return Skills[SkillIndex]->IsStart ();
}

