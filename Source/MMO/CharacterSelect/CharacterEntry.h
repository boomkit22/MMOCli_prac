// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/CharacterTypes.h"
#include "CharacterEntry.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UCharacterEntry : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
	void Init(ECharacterClassType characterClassType, FString level, FString IDText);
	void SetPlayerID(int64 playerID);
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* IDText;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY(meta= (BindWidget))
	class UButton* CharacterButton;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponImage")
	class UTexture2D* AxeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponImage")
	class UTexture2D* SwordImage;

	UFUNCTION()
	void OnSelectButtonClicked();

	UFUNCTION()
	void OnCharacterButtonClicked();

	//������ ĳ���� ��ȯ�ϴ� �� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AActor> GameCharacterClass;

	class AGameCharacter* SpawnedCharacter = nullptr;


private:
	int64 PlayerID;
	ECharacterClassType CharacterClassType = ECharacterClassType::CCT_None;

};
