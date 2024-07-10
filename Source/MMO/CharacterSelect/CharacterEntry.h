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
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* IDText;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponImage")
	class UTexture2D* AxeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponImage")
	class UTexture2D* SwordImage;


};
