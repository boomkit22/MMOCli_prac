// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MMOOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UMMOOverlay::SetHealthBarPercent(float Percent)
{
	if(HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UMMOOverlay::SetStaminaBarPercent(float Percent)
{
	if(StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void UMMOOverlay::SetExperienceBarPercent(float Percent)
{
	if(ExperienceProgressBar)
	{
		ExperienceProgressBar->SetPercent(Percent);
	}
}

void UMMOOverlay::SetLevelTextBlock(int32 Level)
{
	if(LevelTextBlock)
	{
		FString string = FString::FromInt(Level);
		FText text = FText::FromString(string);
		LevelTextBlock->SetText(text);
	}
}
