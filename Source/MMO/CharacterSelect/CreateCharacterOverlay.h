// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateCharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UCreateCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SwordClassButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* AxeClassButton;

	// YourCharacterClass 변수를 TSubclassOf를 사용하여 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> GameCharacterClass;

private:
	UFUNCTION()
	void OnSwordClassButtonclicked();

	UFUNCTION()
	void OnAxeClassButtonClicked();
};
