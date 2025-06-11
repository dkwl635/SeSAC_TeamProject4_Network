// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class WITCHIT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps )  const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//플레이어의 시야값 가져오는
	void GetPlayerViewport ( FVector& Location , FRotator& ViewRot );
	//조종시
	virtual void PossessedBy ( AController* NewController ) override;

	virtual void NotifyControllerChanged () override;
public :
	virtual void StartSkillA ();
	virtual void TriggerSkillA ();
	virtual void CompletedSkillA ();

	virtual void StartSkillB ();
	virtual void TriggerSkillB ();
	virtual void CompletedSkillB ();

	virtual void OnDamage ( int32 damage );

	virtual void EndGame ();
	void CreateInGameUI ();

	//서버 동기화 용
	//1차 서버에서 캐릭터 Init 서버에서 만 호출될 예정
	void SetCreateInit (const  struct FCharacterInfo CharacterInfo );
	//2차 클라에서 캐릭터 Init 서버에서 ->클라로 호출 예정

	UFUNCTION ( Client , Reliable )
	void  ClientRPC_SetCreateInit ( const struct FCharacterInfo CharacterInfo );


	UFUNCTION ( Server , Reliable )
	void ServerRPC_SetSuccessCharacter (  );

	UFUNCTION ( Client , Reliable )
	void ClientRPC_SetLocation ( FVector Postioion );

private:

	void ToggleTeamUI ();

public:
	UPROPERTY(EditAnywhere, Category = Skill)
	class USkillComponent* SkillComp;

	UPROPERTY(EditAnywhere, Category = Input)
	class UWIInputComponent* WitchInputComp;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite , Category = Component )
	class UStateComponent* StateComp;

	UPROPERTY(VisibleAnywhere, Replicated)
	FString OwnerNickName;
private:
	

protected :
	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class UUserWidget> CharacterUIClass;

	UPROPERTY ()
	class UUserWidget* InGameUI;

	UPROPERTY ( EditDefaultsOnly  )
	TSubclassOf<class UPlayerListUI> TeamUIClass;

	UPROPERTY ()
	class UPlayerListUI* TeamUI;

	bool bWitch = false;
};
