// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class WITCHIT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_createRoom; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* edit_roomName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class USlider* slider_playerCount; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* txt_playerCount;

	UPROPERTY()
	class UNetGameInstance* gi; 

public:
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher; 

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* Btn_CreateSession;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UButton* Btn_FindSession;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_back;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* btn_back_1;

	UPROPERTY(BlueprintReadOnly, meta =(BindWidget))
	class UButton* btn_find; 


public:
	virtual void NativeConstruct () override;

	UFUNCTION()
	void CreateRoom();

	UFUNCTION()
	void OnValueChanged ( float Value );

	
	UFUNCTION ()
	void SwitchCreatePanel ();

	UFUNCTION()
	void SwitchFindPanel();

	UFUNCTION()
	void BackToMain();

	// 세션 슬롯
	// canvas_findRoom의 스크롤 박스 위젯

	UPROPERTY(BlueprintReadOnly, meta =(BindWidget))
	class UScrollBox* scroll_roomList; 

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USessionSlotWidget> sessionInfoWidget;

	UFUNCTION()
	void AddSlotWidget(const struct FSessionInfo& sessionInfo);

	UFUNCTION ()
	void OnClickedFindSession ();

	// 방창기 상태 이벤트 콜백
	UFUNCTION()
	void OnChangeButtonEnble(bool bIsSearching);

	UPROPERTY (BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* txt_findingMsg;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UEditableText* edit_userName; 

};
