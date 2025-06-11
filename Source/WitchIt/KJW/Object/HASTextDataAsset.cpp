// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Object/HASTextDataAsset.h"
#include "KJW/Widget/KTextBlock.h"
#include "Engine/AssetManager.h"

UHASTextDataAsset* UHASTextDataAsset::SingletonInstance = nullptr;


UHASTextDataAsset* UHASTextDataAsset::GetInstance ()
{
	// 인스턴스가 없으면 생성하고 반환
	if (!SingletonInstance)
	{
		UE_LOG ( LogTemp , Warning , TEXT ( "Singleton instance is null. Try to load the asset." ) );
		// 싱글턴을 초기화하면서 에셋을 로드
		InitializeSingleton ();
	}
	return SingletonInstance;
}

void UHASTextDataAsset::InitializeSingleton ()
{
	// 이미 에셋을 로드한 인스턴스가 있는지 확인
	if (!SingletonInstance)
	{
		// 경로에 맞는 에셋을 로드
		const FString AssetPath = TEXT ( "/Script/WitchIt.HASTextDataAsset'/Game/KJW/HASDataAsset.HASDataAsset'" );
		SingletonInstance = Cast<UHASTextDataAsset> ( StaticLoadObject ( UHASTextDataAsset::StaticClass () , nullptr , *AssetPath ) );

		if (SingletonInstance)
		{
			// 로딩 성공 시 로그
			SingletonInstance->CacheTagToTextMap ();
			UE_LOG ( LogTemp , Warning , TEXT ( "Successfully loaded asset: %s" ) , *AssetPath );
		}
		else
		{
			// 로딩 실패 시 로그
			UE_LOG ( LogTemp , Error , TEXT ( "Failed to load asset: %s" ) , *AssetPath );
		}
	}
}


FString UHASTextDataAsset::GetTextByIndex ( int32 Key )
{
	if (Language == 0)
	{
		if (const FString* Result = KeyKorTextMap.Find ( Key ))
		{
			return *Result;
		}
	}
	else
	{
		if (const FString* Result = KeyEngTextMap.Find ( Key ))
		{
			return *Result;
		}
	}


	return FString ( TEXT ( "Text Not Found" ) );
}

void UHASTextDataAsset::CacheTagToTextMap ()
{
	if (!TextDataTable) return;
	UE_LOG ( LogTemp , Warning , TEXT ( "CacheTagToTextMap" ) );

	KeyKorTextMap.Empty ();
	KeyEngTextMap.Empty ();

	TArray<FHASTextRow*> AllRows;
	TextDataTable->GetAllRows<FHASTextRow> ( TEXT ( "Text Cache" ) , AllRows );

	for (FHASTextRow* Row : AllRows)
	{
		if (Row)
		{
			KeyKorTextMap.Add ( Row->Text_ID , Row->Text_Kor );
			KeyEngTextMap.Add ( Row->Text_ID , Row->Text_Eng );
			UE_LOG ( LogTemp , Warning , TEXT ( "Text  %d : %s" ) , Row->Text_ID , *Row->Text_Kor );
		}
	}
}
