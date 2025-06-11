// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HASTextGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS( Blueprintable )
class WITCHIT_API UHASTextGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


	///Script/Engine.DataTable'/Game/KJW/DT_Text.DT_Text'
	
public:
	UHASTextGameInstanceSubsystem ();
	virtual void Initialize ( FSubsystemCollectionBase& Collection ) override;

	// DataTable 세팅
	UPROPERTY ( EditDefaultsOnly , Category = "KText" )
	class UDataTable* TextDataTable;

	// 텍스트 조회 함수
	FString GetTextByIndex ( int32 Key );


	//0, 한국어 , 영어
	int32 Language = 0;

private:
	void CacheTagToTextMap ();
	TMap<int32 , FString> KeyKorTextMap;
	TMap<int32 , FString> KeyEngTextMap;


	bool IsSet = false;
};
