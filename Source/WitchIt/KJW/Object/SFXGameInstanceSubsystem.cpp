// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Object/SFXGameInstanceSubsystem.h"
#include "Engine/DataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

bool USFXGameInstanceSubsystem::ShouldCreateSubsystem ( UObject* Outer ) const
{
	//c++ 클래스 가 아닌 블루프린트로 만든것으로 생성을 위한
	if (this->GetClass ()->IsInBlueprint () && Super::ShouldCreateSubsystem ( Outer ))
	{
		return true;
	}

    return false;
}

void USFXGameInstanceSubsystem::Initialize ( FSubsystemCollectionBase& Collection )
{
	Super::Initialize (Collection);

	SetSoundTable ();
	SetEffectTable ();
}

void USFXGameInstanceSubsystem::Deinitialize ()
{


	Super::Deinitialize ();
}

void USFXGameInstanceSubsystem::PlaySound ( UWorld* World , FVector Location , int32 SoundID )
{
	if (!SoundMap.Contains ( SoundID ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Sound Index : %d" ) , SoundID);
		return;
	}

	USoundBase* sound = SoundMap[SoundID];
	if (sound)
	{
		UGameplayStatics::PlaySoundAtLocation ( World , sound , Location );
	}

}

UNiagaraComponent* USFXGameInstanceSubsystem::SpawnEffect ( UWorld* World , FVector Location , FRotator Rotation , int32 EffectID , float time )
{
	if (!EffectMap.Contains ( EffectID ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Effect Index : %d" ) , EffectID );
		return nullptr;
	}

	UNiagaraSystem* effect = EffectMap[EffectID];

	if (effect)  // UNiagaraSystem* 타입
	{
		UNiagaraComponent*  spawn =  UNiagaraFunctionLibrary::SpawnSystemAtLocation (
			GetWorld () ,
			effect ,
			Location ,  // 스폰 위치
			Rotation ,  // 스폰 회전
			FVector ( 1.0f ) ,       // 스케일
			true ,                // AutoDestroy
			true ,                // AutoActivate
			ENCPoolMethod::None , // 풀링 여부
			true                 // PreCullCheck
		);

		return spawn;
	}

	return nullptr;
}



void USFXGameInstanceSubsystem::PlaySound2D ( UWorld* World , int32 SoundID )
{
	if (!SoundMap.Contains ( SoundID ))
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "No Sound Index : %d" ) , SoundID );
		return;
	}

	USoundBase* sound = SoundMap[SoundID];
	if (sound)
	{
		UGameplayStatics::PlaySound2D ( World , sound  );
	}
}

void USFXGameInstanceSubsystem::SetSoundTable ()
{
	if (!SoundDataTable) return;

	SoundMap.Empty ();
	

	TArray<FSoundRow*> AllRows;
	SoundDataTable->GetAllRows<FSoundRow> ( TEXT ( "Text Cache" ) , AllRows );

	UE_LOG ( LogTemp , Warning , TEXT ( "CacheTagToSoundMap" ) );
	
	for (FSoundRow* Row : AllRows)
	{
		if (Row)
		{
			SoundMap.Add ( Row->Sound_ID , Row->Sound );
		
			UE_LOG ( LogTemp , Warning , TEXT ( "Text  %d : %s" ) , Row->Sound_ID , *(Row->Sound->GetFName().ToString()));
		}
	}


}

void USFXGameInstanceSubsystem::SetEffectTable ()
{
	if (!EffectDataTable) return;

	EffectMap.Empty ();


	TArray<FEffectRow*> AllRows;
	EffectDataTable->GetAllRows<FEffectRow> ( TEXT ( "Text Cache" ) , AllRows );

	UE_LOG ( LogTemp , Warning , TEXT ( "CacheTagToSoundMap" ) );

	for (FEffectRow* Row : AllRows)
	{
		if (Row)
		{
			EffectMap.Add ( Row->Effect_ID , Row->NiagaraSystem );

			UE_LOG ( LogTemp , Warning , TEXT ( "Text  %d : %s" ) , Row->Effect_ID , *(Row->NiagaraSystem->GetFName ().ToString ()) );
		}
	}
}
