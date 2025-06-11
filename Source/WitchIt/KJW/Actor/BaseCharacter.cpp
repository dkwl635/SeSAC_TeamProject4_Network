// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Actor/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "KJW/Actor/HASPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KJW/ActorComponent/SkillComponent.h"
#include "SSA/ActorComponent/WIInputComponent.h"
#include "SSA/ActorComponent/StateComponent.h"
#include  "KJW/UI/PlayerListUI.h"


#include "KJW/GameMode/HASGameState.h"
#include "KJW/GameMode/HASPlayerState.h"
#include "KJW/GameMode/HideAndSeekGameMode.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SkillComp = CreateDefaultSubobject<USkillComponent> (TEXT("SkillComp"));
	WitchInputComp = CreateDefaultSubobject<UWIInputComponent> ( TEXT ( "WitchInputComp" ) );
	StateComp = CreateDefaultSubobject<UStateComponent> ( TEXT ( "StateComp" ) );

	SetReplicates ( true );
	SetReplicateMovement ( true );
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled ())
	{
		
		SkillComp->InitSkillComponent ();
	}
	

	
}

void ABaseCharacter::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( ABaseCharacter , OwnerNickName );
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent> ( PlayerInputComponent );

	if (IsValid ( EnhancedInputComponent ))
	{

		//스킬 입력 바인딩
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Skill1 , ETriggerEvent::Started , this , &ABaseCharacter::StartSkillA );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Skill1 , ETriggerEvent::Triggered , this , &ABaseCharacter::TriggerSkillA );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Skill1 , ETriggerEvent::Completed , this , &ABaseCharacter::CompletedSkillA );

		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Skill2 , ETriggerEvent::Started , this , &ABaseCharacter::StartSkillB );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Skill2 , ETriggerEvent::Triggered , this , &ABaseCharacter::TriggerSkillB );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Skill2 , ETriggerEvent::Completed , this , &ABaseCharacter::CompletedSkillB );

		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Tab , ETriggerEvent::Started , this , &ABaseCharacter::ToggleTeamUI );
		EnhancedInputComponent->BindAction ( WitchInputComp->IA_Tab , ETriggerEvent::Completed , this , &ABaseCharacter::ToggleTeamUI );
	}
}

void ABaseCharacter::GetPlayerViewport(FVector& Location , FRotator& ViewRot)
{
	GetController()->GetPlayerViewPoint(Location, ViewRot);
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	auto* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsystem)
		{
			subsystem->ClearAllMappings(); // 선택: 기존 맵핑 제거
			subsystem->AddMappingContext(WitchInputComp->IMC_WitchIt, 0);
		}
	}

	//게임 시작시로 옮기긴 해야함
	//CreateInGameUI();
}

void ABaseCharacter::NotifyControllerChanged ()
{
	Super::NotifyControllerChanged ();

	auto* pc = Cast<APlayerController> ( Controller );
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( pc->GetLocalPlayer () );

		if (subsystem)
		{
			subsystem->ClearAllMappings (); // 선택: 기존 맵핑 제거
			subsystem->AddMappingContext ( WitchInputComp->IMC_WitchIt , 0 );
		}
	}


}

void ABaseCharacter::StartSkillA ()
{
	SkillComp->StartSkillA ();
	//추후 스킬 UI 갱신 등 작업 추가하기
}

void ABaseCharacter::TriggerSkillA ()
{
	SkillComp->TriggerSkillA ();
}

void ABaseCharacter::CompletedSkillA ()
{
	SkillComp->CompletedSkillA ();
}

void ABaseCharacter::StartSkillB ()
{
	SkillComp->StartSkillB ();
}

void ABaseCharacter::TriggerSkillB ()
{
	SkillComp->TriggerSkillB ();
}

void ABaseCharacter::CompletedSkillB ()
{
	SkillComp->CompletedSkillB ();
}

void ABaseCharacter::OnDamage ( int32 damage )
{
}

void ABaseCharacter::EndGame ()
{
	if (InGameUI)
	{
		InGameUI->RemoveFromParent ();
	}
}

void ABaseCharacter::CreateInGameUI()
{
	auto* pc = Cast<APlayerController>(Controller);
	InGameUI = CreateWidget<UUserWidget>(pc, CharacterUIClass);
	if (InGameUI)
	{
		InGameUI->AddToViewport();
	}

	TeamUI = CreateWidget<UPlayerListUI> ( pc , TeamUIClass );
	if (TeamUI)
	{
		TeamUI->AddToViewport ();
		TeamUI->SetVisibility ( ESlateVisibility::Collapsed );
		TeamUI->SetPlayersInfo ();
	}

}

void ABaseCharacter::SetCreateInit ( const FCharacterInfo CharacterInfo )
{
	
	SkillComp->SkillActors.Empty ();
	AHASGameState* GS = GetWorld ()->GetGameState<AHASGameState> ();

	if (GS)
	{
		TSubclassOf<USkillBase> SkillA = CharacterInfo.bWitch ? GS->WitchSkills[CharacterInfo.SkillA_Index] : GS->HunterSkills[CharacterInfo.SkillA_Index];
		TSubclassOf<USkillBase> SkillB = CharacterInfo.bWitch ? GS->WitchSkills[CharacterInfo.SkillB_Index] : GS->HunterSkills[CharacterInfo.SkillB_Index];
		SkillComp->SkillActors.Add ( SkillA );
		SkillComp->SkillActors.Add ( SkillB );
		SkillComp->InitSkillComponent ();
	}

	ClientRPC_SetCreateInit ( CharacterInfo );
}

void ABaseCharacter::ServerRPC_SetSuccessCharacter_Implementation ( )
{
	AHASGameState* GS = GetWorld ()->GetGameState<AHASGameState> ();
	if (GS)
		GS->SetSuccessCharacter ( );
}

void ABaseCharacter::ClientRPC_SetCreateInit_Implementation ( const  FCharacterInfo CharacterInfo )
{
	// Create in-game UI (Optional - server-only or need to replicate it?)
	//

	SkillComp->SkillActors.Empty ();
	AHASGameState* GS = GetWorld ()->GetGameState<AHASGameState> ();

	if (GS)
	{
		TSubclassOf<USkillBase> SkillA = CharacterInfo.bWitch ? GS->WitchSkills[CharacterInfo.SkillA_Index] : GS->HunterSkills[CharacterInfo.SkillA_Index];
		TSubclassOf<USkillBase> SkillB = CharacterInfo.bWitch ? GS->WitchSkills[CharacterInfo.SkillB_Index] : GS->HunterSkills[CharacterInfo.SkillB_Index];
		SkillComp->SkillActors.Add ( SkillA );
		SkillComp->SkillActors.Add ( SkillB );
		SkillComp->InitSkillComponent ();

		ServerRPC_SetSuccessCharacter ( );
	}

	CreateInGameUI ();
}


void ABaseCharacter::ToggleTeamUI ()
{
	
	if (TeamUI->GetVisibility () == ESlateVisibility::Collapsed)
	{
		TeamUI->SetVisibility ( ESlateVisibility::Visible );
	}
	else
	{
		TeamUI->SetVisibility ( ESlateVisibility::Collapsed );
	}

}


void ABaseCharacter::ClientRPC_SetLocation_Implementation ( FVector Postioion )
{
	//SetActorLocation ( Postioion );

	FVector Dir = Postioion;
	float Strength = 1500.0f; 
	LaunchCharacter ( Dir * Strength , true , true );
}
