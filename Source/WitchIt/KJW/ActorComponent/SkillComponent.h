// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KJW/Object/SkillBase.h"
#include "SkillComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WITCHIT_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	friend class ABaseCharacter;

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent () override;
	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps )  const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//시작시 스킬컴포넌트 셋팅
	void InitSkillComponent ();

	//스킬 사용 함수 -> 실제 2개의 스킬만 사용하니
	void StartSkillA ();
	void TriggerSkillA ();
	void CompletedSkillA ();

	void StartSkillB ();
	void TriggerSkillB ();
	void CompletedSkillB ();
	
	UFUNCTION ( Server , Reliable )
	void ServerRPC_StartSkill ( int32 Index );
	
	UFUNCTION ( Server , Reliable )
	void ServerRPC_CompletedSkill ( int32 Index );

	class USkillBase* GetSkill ( int32 SkillIndex );

public:
	//우선 2개만 
	UPROPERTY ( EditDefaultsOnly , Category = Skill )
	TArray<TSubclassOf<USkillBase>> SkillActors;


	UPROPERTY (Replicated, EditAnywhere)
	bool bTest = false;
private:

	//스킬 들 틱 수행
	void SkillTick ( float DeltaTime );
	bool IsStartSkill ( int32 SkillIndex );

private:


	UPROPERTY( VisibleAnywhere , Instanced, Category = Skill )
	TArray<USkillBase*> Skills;

	UPROPERTY ()
	class ABaseCharacter* OwnerCharacter;


	

};
