// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UCharacterSelectOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* CharacterSelectButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CharacterNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CharacterLevelText;

public:
	void SetCharacterSelectText(const FString& CharacterName, uint16 CharacterLevel);
protected:
	// bind하는건 UFUNCTION안하면 에러남
	UFUNCTION()
	void OnCharacterSelectButtonClicked();
};
