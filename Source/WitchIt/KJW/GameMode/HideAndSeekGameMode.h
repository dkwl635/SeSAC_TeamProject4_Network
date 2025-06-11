// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HideAndSeekGameMode.generated.h"

/**
 * 
 */
UCLASS ()
class WITCHIT_API AHideAndSeekGameMode : public AGameModeBase
{
	GENERATED_BODY ()

public:
	virtual void BeginPlay () override;

	void ReStartGame ();

public:
	UPROPERTY ( EditDefaultsOnly )
	FName HASLevel;

	FSimpleMulticastDelegate OnPlayerStateUpdate;

public :
	void SetSessionGameState ( int32 Index );
	int32 AllPlayer = 0;
private:
	virtual void PostLogin ( APlayerController* NewPlayer ) override;
	virtual void Logout ( AController* Exiting )override;
	
private:
	bool bMoveLevel = false;

	UPROPERTY()
	class UNetGameInstance* NetGameIns;

};
