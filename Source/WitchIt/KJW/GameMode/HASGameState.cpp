// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/GameMode/HASGameState.h"
#include "WitchIt.h"
#include "KJW/GameMode/HideAndSeekGameMode.h"
#include "KJW/GameMode/HASPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "KJW/Actor/Hunter.h"
#include "KJW/Actor/HASPlayerController.h"
#include "SSA/Character/Witch.h"
#include "SSA/Actor/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "KJW/UI/KillLogUI.h"
#include "KJW/Actor/SpawnPoint.h"
#include "Components/BoxComponent.h"

#include "SSA/GameInstance/NetGameInstance.h"

#include "KJW/Actor/Wall.h"


AHASGameState::AHASGameState ()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AHASGameState::BeginPlay ()
{
	Super::BeginPlay ();


	if (!HasAuthority ())return;


	GM = GetWorld ()->GetAuthGameMode<AHideAndSeekGameMode> ();
	if (GM)
	{
		GM->OnPlayerStateUpdate.AddUObject ( this , &AHASGameState::OnPlayerStateUpdate );
	}
	OnPlayerStateUpdate ();
	Server_ChangeGameState ( EHASGameState::READY );


}

void AHASGameState::GetLifetimeReplicatedProps ( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps ( OutLifetimeProps );

	DOREPLIFETIME ( AHASGameState , HASGameState );
	DOREPLIFETIME ( AHASGameState , HideTimer );
	DOREPLIFETIME ( AHASGameState , GameTimer );
	DOREPLIFETIME ( AHASGameState , NextGameTimer );

	DOREPLIFETIME ( AHASGameState , PlayerWitch );
	DOREPLIFETIME ( AHASGameState , PlayerHunter );
	DOREPLIFETIME ( AHASGameState , AllPlayer );

	DOREPLIFETIME ( AHASGameState , bWitchWin );

}

void AHASGameState::Tick(float DeltaSeconds)
{
	
	//서버만
	if (!HasAuthority ()) return;

	if (HASGameState == EHASGameState::HIDE)
	{
		HideTimer -= DeltaSeconds;
		if (HideTimer<= 0.0f)
		{
			HideTimer = 0.0f;
			Server_ChangeGameState ( EHASGameState::SEEK );
		}
	}
	else if (HASGameState == EHASGameState::READY)
	{ 
	}
	else if (HASGameState == EHASGameState::SEEK)
	{
		GameTimer -= DeltaSeconds;

		if (GameTimer <= 0)
		{
			GameTimer = 0.0f;
			Server_ChangeGameState ( EHASGameState::END );
		}
	}
	else if (HASGameState == EHASGameState::END)
	{
		NextGameTimer -= DeltaSeconds;
		if (NextGameTimer <= 0)
		{
			NextGameTimer = 0.0f;
			Server_ChangeGameState ( EHASGameState::MAPMOVE );
		}
	}
}

void AHASGameState::StartState ()
{
	if (IsStartGame) return;

	IsStartGame = true;
	//나중에 는 모든 클라이언트가 준비 되거나... 대기 시간 종료시 HIDE 타임으로 넘어가기
	//ChangeGameState ( EHASGameState::READY );
	Server_ChangeGameState ( EHASGameState::HIDE );

}

void AHASGameState::DieWitch ( APlayerController* WitchPc )
{
	
	--AlliveWitch;
	if (AlliveWitch == 0)
	{
		bWitchWin = false;
		Server_ChangeGameState ( EHASGameState::END );
		//EndGame ();
	}
}



void AHASGameState::SetCharacterCount ()
{
	int32 NewWitchCount = 0;
	int32 NewHunterCount = 0;

	for (APlayerState* PS : PlayerArray)
	{
		AHASPlayerState* HASPS = Cast<AHASPlayerState> ( PS );
		if (HASPS)
		{
			if (HASPS->CharacterType == 2)
				NewWitchCount++;
			else if (HASPS->CharacterType == 1)
				NewHunterCount++;
		}
	}
	
	PlayerWitch = NewWitchCount;
	PlayerHunter = NewHunterCount;
	OnPlayerUpdate.Broadcast ();
}

void AHASGameState::DieWitchLog ( FString HunterName , FString WitchName )
{
	

	MulticastRPC_DieWitchLog ( HunterName , WitchName );
}

TArray<TObjectPtr<APlayerState>>* AHASGameState::GetPlayerStates ()
{

	return nullptr;
}

FVector AHASGameState::GetSpawnPoint ( bool bWitch )
{
	
	ASpawnPoint* point = bWitch ? WitchSpawner : HunterSpawner;

	if (!point)	return FVector::ZeroVector;

	FVector spawnPoint = point->GetActorLocation ();

	FVector boxExtent =	point->BoxComp->GetUnscaledBoxExtent ();
	spawnPoint.X = spawnPoint.X + (FMath::FRandRange ( -boxExtent.X , boxExtent.X ));
	spawnPoint.Y = spawnPoint.Y + (FMath::FRandRange ( -boxExtent.Y , boxExtent.Y ));

	return spawnPoint;
}

void AHASGameState::EndGame()
{

	//나중에 서버 쪽에서 모든   클라에게 전달해야 할 내용
	AHASPlayerController* pc = GetWorld ()->GetFirstPlayerController<AHASPlayerController> ();
	if (pc)
	{
		pc->OpenEndGameResultUI ();	
	}
}

void AHASGameState::MoveLevel ()
{
	if (GM)
	{
		GM->ReStartGame ();
	}
}

void AHASGameState::SetSpawner ()
{
	TArray<AActor*> find;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , ASpawnPoint::StaticClass () , find );
	for (AActor* actor : find)
	{
		ASpawnPoint* point = Cast<ASpawnPoint> ( actor );
		if (point)
		{
			if (point->bWitchSpawnPoint)
			{
				WitchSpawner = point;
			}
			else
			{
				HunterSpawner = point;
			}
		}
	}

}

void AHASGameState::DestroyWall ()
{
	TArray<AActor*> Walls;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AWall::StaticClass () , Walls );
	for (AActor* wall : Walls)
	{
		wall->Destroy ();
	}

}

void AHASGameState::OnPlayerStateUpdate ()
{
	if (GM)
	{
		//본인 첫 서버는 호출 안됨
		AllPlayer = GM->AllPlayer;
		UE_LOG ( LogTemp , Log , TEXT ( "플레이어들 갱신 %d" ) , AllPlayer );

		OnPlayerUpdate.Broadcast ();
	}

}

void AHASGameState::OnRep_UpdatePlayer ()
{
	PRINTLOG ( TEXT ( "AllPlayer 값이 변경되었습니다! 새 값: %d" ) , AllPlayer );
	PRINTLOG ( TEXT ( "헌터 값이 변경되었습니다! 새 값: %d" ) ,PlayerHunter );
	PRINTLOG ( TEXT ( "마녀 값이 변경되었습니다! 새 값: %d" ) , PlayerWitch );
	
	OnPlayerUpdate.Broadcast ();
}

void AHASGameState::OnRep_UpdateState ()
{
	switch (HASGameState)
	{
	case EHASGameState::END:
	{
		EndGame ();
		break;
	}
	default:
		break;
	}
}

void AHASGameState::CloseSession ()
{
	
	if (GM)
	{
		GM->SetSessionGameState ( 1 );
	}
}

void AHASGameState::MulticastRPC_DieWitchLog_Implementation ( const FString& HunterName , const FString& WitchName )
{
	if (KillLogUI)
	{
		KillLogUI->ShowKillLog ( HunterName , WitchName );
	}
}



void AHASGameState::Server_ChangeGameState ( EHASGameState NewState )
{
	if (HASGameState == NewState) return;

	HASGameState = NewState;

	switch (HASGameState)
	{
	case EHASGameState::READY:
	{
		
		SetItems ();
		SetSpawner ();
		break;
	}
	case EHASGameState::HIDE:
	{
		CloseSession ();
		SetHunters ();
		SetWitches ();

		break;
	}
	case EHASGameState::SEEK:
	{
		DestroyWall ();
		break;
	}
	case EHASGameState::END:
	{
		EndGame ();
		break;
	}
	case EHASGameState::MAPMOVE:
	{
	
		if (GM)
		{
			GM->ReStartGame ();
		}
		break;
	}
	default:
		break;
	}
}


void AHASGameState::SetSuccessCharacter ()
{
	CreateSuccessPlayer++;

	//결국 컨트롤러의 수 만큼 완료 하면 
	if (CreateSuccessPlayer == GetWorld ()->GetNumPlayerControllers ())
	{
		StartState ();
	}

}



void AHASGameState::SetSelectCharacter_Implementation ( bool bWitch )
{
	if (bWitch)
	{
		PlayerWitch++;
	}
	else
	{
		PlayerHunter++;
	}

}

void AHASGameState::SetHunters ()
{
	Hunters.Empty ();

	TArray<AActor*> Finds;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AHunter::StaticClass () , Finds );
	for (AActor* find : Finds)
	{
		if (AHunter* hunter = Cast<AHunter> ( find ))
		{
			Hunters.Add ( hunter );
		}
	}

	UE_LOG ( LogTemp , Warning , TEXT ( "Set Hunters : %d" ) , Hunters.Num () );

	PlayerHunter = Hunters.Num ();

}

void AHASGameState::SetWitches ()
{
	Witches.Empty ();

	TArray<AActor*> Finds;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AWitch::StaticClass () , Finds );
	for (AActor* find : Finds)
	{
		if (AWitch* Witch = Cast<AWitch> ( find ))
		{
			Witches.Add ( Witch );
		}
	}

	UE_LOG ( LogTemp , Warning , TEXT ( "Set Witches : %d" ) , Witches.Num () );
	PlayerWitch = Witches.Num ();
	AlliveWitch = PlayerWitch;
}

void AHASGameState::SetItems ()
{
	Items.Empty ();

	TArray<AActor*> Finds;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld () , AItemBase::StaticClass () , Finds );
	for (AActor* find : Finds)
	{
		if (AItemBase* Item = Cast<AItemBase> ( find ))
		{
			Items.Add ( Item );
		}
	}

	UE_LOG ( LogTemp , Warning , TEXT ( "Set Items : %d" ) , Items.Num () );

}
