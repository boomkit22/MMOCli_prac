// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDMonsterComponent.h"
#include "HUD/HUDMonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDMonsterComponent::SetHealthPercent(float Percent)
{
	if (MonsterWidget == nullptr)
	{
		MonsterWidget = Cast<UHUDMonsterWidget>(GetUserWidgetObject());
	}

	if (MonsterWidget && MonsterWidget->HealthBar)
	{
		MonsterWidget->HealthBar->SetPercent(Percent);
	}
}

void UHUDMonsterComponent::SetMonsterName(FString monsterName)
{
	if (MonsterWidget == nullptr)
	{
		MonsterWidget = Cast<UHUDMonsterWidget>(GetUserWidgetObject());
	}

	if (MonsterWidget && MonsterWidget->MonsterName)
	{
		MonsterWidget->MonsterName->SetText(FText::FromString(monsterName));
	}
}