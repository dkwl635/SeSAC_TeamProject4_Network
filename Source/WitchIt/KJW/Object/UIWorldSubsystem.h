// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UUIWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize ( FSubsystemCollectionBase& Collection ) override;
	virtual void Deinitialize () override;

public:
	void ChangeLanguage ( int32 LanguageIndex );

public:
	FSimpleMulticastDelegate OnChangeLanguage;


};
