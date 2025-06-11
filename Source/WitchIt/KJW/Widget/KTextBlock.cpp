// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Widget/KTextBlock.h"
#include "KJW/Object/HASTextGameInstanceSubsystem.h"
#include "KJW/Object/HASTextDataAsset.h"
#include "KJW/Object/UIWorldSubsystem.h"

//#if WITH_EDITOR
#include "Editor.h"  
#include "Editor/UnrealEdEngine.h" 
#include "UnrealEdGlobals.h"       
//#endif

void UKTextBlock::OnWidgetRebuilt ()
{
	Super::OnWidgetRebuilt ();
	
	
	UWorld* world = GetWorld ();
	if (world && world->IsGameWorld ())
	{
		UpdateTextFromTag ();
		UUIWorldSubsystem *subsystem = world->GetSubsystem<UUIWorldSubsystem> ();
		if (subsystem)
		{
			subsystem->OnChangeLanguage.AddUObject ( this , &UKTextBlock::UpdateTextFromTag );
		}
	}
}

#if WITH_EDITOR
void UKTextBlock::PostEditChangeProperty ( FPropertyChangedEvent& PropertyChangedEvent )
{
	Super::PostEditChangeProperty ( PropertyChangedEvent );

	if (Text_ID == 0)
		return;
	const FName ChangedProp = PropertyChangedEvent.GetPropertyName ();
	if (ChangedProp == GET_MEMBER_NAME_CHECKED ( UKTextBlock , Text_ID ))
	{
		UpdateTextFromTag ();
	}

}
#endif

void UKTextBlock::UpdateTextFromTag ()
{
	//if (!TextDataTable || !TextTag.IsValid ()) return;

	if (Text_ID == 0)
		return;

	FString str;

	UWorld* world = GetWorld ();
	if (world && world->IsGameWorld ())
	{
		UHASTextGameInstanceSubsystem* sub = GetGameInstance ()->GetSubsystem<UHASTextGameInstanceSubsystem> ();
		if (sub)
		{
			str = sub->GetTextByIndex ( Text_ID );
			if (str.IsEmpty ())
			{
				SetText ( FText::FromString ( TEXT ( "str.IsEmpty" ) ) );
			}
			else
			{
				SetText ( FText::FromString ( str ) );
			}
		}
		else
		{
			SetText ( FText::FromString ( TEXT ( "[[No TextSubsystem]]" ) ) );
		}
	}
	else if(world && world->IsEditorWorld ())
	{
		UHASTextDataAsset* dataAsset = UHASTextDataAsset::GetInstance ();
		if (dataAsset)
		{
			str = dataAsset->GetTextByIndex ( Text_ID );
			if (str.IsEmpty ())
			{
				SetText ( FText::FromString ( TEXT ( "str.IsEmpty" ) ) );
			}
			else
			{
				SetText ( FText::FromString ( str ) );
			}
		}
		else
		{
			SetText ( FText::FromString ( TEXT ( "[[No TextSubsystem]]" ) ) );
		}
	}

}
