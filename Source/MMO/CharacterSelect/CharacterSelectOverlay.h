// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <vector>
#include "Type.h"
#include "CharacterSelectOverlay.generated.h"

/**
 * 
 */
class UCreateCharacterOverlay;
class UCharacterEntry;
UCLASS()
class MMO_API UCharacterSelectOverlay : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UCreateCharacterOverlay> UCreateCharacterOverlayClass;


	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UCharacterEntry> UChaterEntryClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* CreateButton;
	
	void SpawnChracter(ECharacterClassType CharacterClassType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> GameCharacterClass;

	class AGameCharacter* SpawnedCharacter = nullptr;

public:
	// 새 항목을 추가하는 메서드
	UFUNCTION(BlueprintCallable, Category = "Entry")
	void AddCharacterEntry(class UCharacterEntry* NewEntry);
	void AddCharacterEntry(PlayerInfo playerInfo);
	void SetCharacterList(std::vector<PlayerInfo>& playerInfos);
protected:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CharacterListVerticalBox;

	UFUNCTION()
	void OnCreateButtonClicked();
};
