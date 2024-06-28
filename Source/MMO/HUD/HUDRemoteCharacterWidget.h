// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDRemoteCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UHUDRemoteCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterLevel;
};
