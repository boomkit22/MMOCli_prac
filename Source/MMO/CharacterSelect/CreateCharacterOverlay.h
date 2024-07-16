// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectOverlay.h"
#include "CreateCharacterOverlay.generated.h"

/**
 * 
 */

class UCharacterEntry;

UCLASS()
class MMO_API UCreateCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

public:
	void SetCharacterSelectOverlay(UCharacterSelectOverlay* CharacterSelectOverlay);
	UCharacterSelectOverlay* CharacterSelectOverlay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* SwordClassButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* AxeClassButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* OkButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IDTextBox;

	// YourCharacterClass 변수를 TSubclassOf를 사용하여 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> GameCharacterClass;

private:
	UFUNCTION()
	void OnSwordClassButtonclicked();

	UFUNCTION()
	void OnAxeClassButtonClicked();

	UFUNCTION()
	void OnOkButtonClicked();

	class AGameCharacter* SpawnedCharacter = nullptr;
	ECharacterClassType CharacterClassType = ECharacterClassType::CCT_None;

	UPROPERTY(EditDefaultsOnly, Category = "Entry")
	TSubclassOf<UCharacterEntry> UCharacterEntryClass;
};
														