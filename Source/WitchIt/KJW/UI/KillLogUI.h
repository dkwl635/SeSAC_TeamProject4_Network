// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillLogUI.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API UKillLogUI : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY ( BlueprintReadWrite , meta = (BindWidget) )
	class UVerticalBox* Vertical_KillLog;

	UPROPERTY ()
	TArray<class UKillLogSlot*> KillLogSlots;

	UPROPERTY ( EditAnywhere )
	TSubclassOf<class UKillLogSlot> KillLogSlotClass;


public:
	virtual void NativeConstruct () override;
	void ShowKillLog ( FString HunterName , FString WitchName );
	class UKillLogSlot* GetLogSlot();


};
