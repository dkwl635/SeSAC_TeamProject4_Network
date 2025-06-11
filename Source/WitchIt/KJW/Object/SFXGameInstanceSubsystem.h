// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SFXGameInstanceSubsystem.generated.h"

/**
 * 
 */
 // 데이터 테이블용 구조체 예시
USTRUCT ( BlueprintType )
struct FSoundRow : public FTableRowBase
{
	GENERATED_BODY ()

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	int32 Sound_ID = 0;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	USoundBase* Sound;
};

USTRUCT ( BlueprintType )
struct FEffectRow : public FTableRowBase
{
	GENERATED_BODY ()

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	int32 Effect_ID = 0;

	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	class UNiagaraSystem* NiagaraSystem;
};


UCLASS( Blueprintable )
class WITCHIT_API USFXGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


private:
	virtual bool ShouldCreateSubsystem ( UObject* Outer ) const override;

	/** Implement this for initialization of instances of the system */
	virtual void Initialize ( FSubsystemCollectionBase& Collection ) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize () override;


public :
	UPROPERTY ( EditDefaultsOnly )
	int32 index = 0;

	UFUNCTION(BlueprintCallable)
	void PlaySound (UWorld* World ,FVector Location ,int32 SoundID);
	
	UFUNCTION(BlueprintCallable)
	class UNiagaraComponent* SpawnEffect ( UWorld* World , FVector Location , FRotator Rotation, int32 EffectID , float time = 0);

	UFUNCTION ( BlueprintCallable )
	void PlaySound2D ( UWorld* World ,int32 SoundID );
private:
	void SetSoundTable ();
	void SetEffectTable ();

private :

	UPROPERTY ( EditDefaultsOnly , Category = "Sound" )
	class UDataTable* SoundDataTable;

	UPROPERTY ( EditDefaultsOnly , Category = "Effect" )
	class UDataTable* EffectDataTable;

	UPROPERTY()
	TMap<int32 , USoundBase*> SoundMap;

	UPROPERTY ()
	TMap<int32 , class UNiagaraSystem*> EffectMap;
};
