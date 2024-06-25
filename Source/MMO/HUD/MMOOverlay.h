// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MMOOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UMMOOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetExperienceBarPercent(float Percent);
	void SetLevelTextBlock(int32 Level);


private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ExperienceProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelTextBlock;
};
