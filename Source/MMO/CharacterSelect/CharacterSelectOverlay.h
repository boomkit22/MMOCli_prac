// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectOverlay.generated.h"

/**
 * 
 */
class UCreateCharacterOverlay;

UCLASS()
class MMO_API UCharacterSelectOverlay : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
public:

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UCreateCharacterOverlay> UCreateCharacterOverlayClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* CreateButton;
	
public:
	// �� �׸��� �߰��ϴ� �޼���
	UFUNCTION(BlueprintCallable, Category = "Entry")
	void AddCharacterEntry(class UCharacterEntry* NewEntry);

protected:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* CharacterListVerticalBox;

	UFUNCTION()
	void OnCreateButtonClicked();
};
