// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Object/HASTextGameInstanceSubsystem.h"
#include "KJW/Widget/KTextBlock.h"

UHASTextGameInstanceSubsystem::UHASTextGameInstanceSubsystem ()
{
	if (!TextDataTable)
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> TableObj ( TEXT ( "/Script/Engine.DataTable'/Game/KJW/DT_Text.DT_Text'" ) );
		if (TableObj.Succeeded ())
		{
			TextDataTable = TableObj.Object;
		}
	}
}

void UHASTextGameInstanceSubsystem::Initialize ( FSubsystemCollectionBase& Collection )
{
	Super::Initialize ( Collection );
	CacheTagToTextMap ();
}




FString UHASTextGameInstanceSubsystem::GetTextByIndex ( int32 Key )
{
	if (!IsSet)
	{		
		CacheTagToTextMap ();
	}

	if (Language == 0)
	{
		if (const FString* Result = KeyKorTextMap.Find ( Key ))
		{
			return *Result;
		}
	}
	else if (Language == 1)
	{ 
		if (const FString* Result = KeyEngTextMap.Find ( Key ))
		{
			return *Result;
		}
	}


	return FString::Printf ( TEXT ( "Text Not Found %d : %d " ) , Language , Key );
}

void UHASTextGameInstanceSubsystem::CacheTagToTextMap ()
{
	if (!TextDataTable) return;

	IsSet = true;
	KeyKorTextMap.Empty ();
	KeyEngTextMap.Empty ();

	TArray<FHASTextRow*> AllRows;
	TextDataTable->GetAllRows<FHASTextRow> ( TEXT ( "Text Cache" ) , AllRows );

	UE_LOG ( LogTemp , Warning , TEXT ( "CacheTagToTextMap" )  );
	for (FHASTextRow* Row : AllRows)
	{
		if (Row)
		{
			KeyKorTextMap.Add ( Row->Text_ID , Row->Text_Kor );
			KeyEngTextMap.Add ( Row->Text_ID , Row->Text_Eng );
			UE_LOG (LogTemp , Warning, TEXT ( "Text  %d : %s ,  %s" ) , Row->Text_ID , *Row->Text_Kor , *Row->Text_Eng);
		}
	}

}
