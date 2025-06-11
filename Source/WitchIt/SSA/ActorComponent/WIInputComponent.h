// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WIInputComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WITCHIT_API UWIInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWIInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputMappingContext* IMC_WitchIt;

	// WASD 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Move;

	// 카메라 회전 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Turn;

	// 점프 'Space' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Jump; 

	// 앉기 'C' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Crouch;

	// 감자공격 / 마녀 변신 '마우스 좌클릭' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Interact;

	// 스킬 1 '마우스 우클릭' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Skill1; 

	// 스킬 2 'Q' 바인딩 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Skill2;

	// 근접 공격 'V' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_MeleeAttack;

	// 도발 '1' 바인딩 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Quiz;

	// 메뉴 키  'ESC' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Menu; 

	// 플레이어 리스트 'Tab' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_Tab; 

	// 카메라 전환 'T' 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* IA_CameraChange;
};
