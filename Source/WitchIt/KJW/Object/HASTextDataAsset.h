// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HASTextDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UHASTextDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 싱글턴 패턴을 위한 정적 함수
	static UHASTextDataAsset* GetInstance ();

	
public:
	// 텍스트 조회 함수
	FString GetTextByIndex ( int32 Key );

public:
	UFUNCTION ( CallInEditor , Category = "CacheText" )
	void Editor_CacheTagToTextMap ()
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "CacheTagToTextMap" )  );
		GetInstance ();
		CacheTagToTextMap ();
	}

	//0, 한국어 , 영어
	UPROPERTY ( EditAnywhere , BlueprintReadWrite )
	int32 Language = 0;

private:
	void CacheTagToTextMap ();
	TMap<int32 , FString> KeyKorTextMap;
	TMap<int32 , FString> KeyEngTextMap;

	// 싱글턴 인스턴스를 위한 static 변수
	static UHASTextDataAsset* SingletonInstance;

	// 싱글턴 인스턴스를 가져오는 함수
	static void InitializeSingleton ();

	// DataTable 세팅
	UPROPERTY ( EditDefaultsOnly , Category = "KText" )
	class UDataTable* TextDataTable;

};
