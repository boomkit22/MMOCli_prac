// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HUDRemoteCharacterComponent.h"
#include "HUD/HUDRemoteCharacterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDRemoteCharacterComponent::SetHealthPercent(float Percent)
{
	if (RemoteCharacterWidget == nullptr)
	{
		RemoteCharacterWidget = Cast<UHUDRemoteCharacterWidget>(GetUserWidgetObject());
	}

	if (RemoteCharacterWidget && RemoteCharacterWidget->HealthBar)
	{
		RemoteCharacterWidget->HealthBar->SetPercent(Percent);
	}
}

void UHUDRemoteCharacterComponent::SetCharacterName(FString Name)
{
	if (RemoteCharacterWidget == nullptr)
	{
		RemoteCharacterWidget = Cast<UHUDRemoteCharacterWidget>(GetUserWidgetObject());
	}

	if (RemoteCharacterWidget && RemoteCharacterWidget->CharacterName)
	{
		RemoteCharacterWidget->CharacterName->SetText(FText::FromString(Name));
	}
}

void UHUDRemoteCharacterComponent::SetCharacterLevel(uint16 Level)
{
	if (RemoteCharacterWidget == nullptr)
	{
		RemoteCharacterWidget = Cast<UHUDRemoteCharacterWidget>(GetUserWidgetObject());
	}

	if (RemoteCharacterWidget && RemoteCharacterWidget->CharacterLevel)
	{
		RemoteCharacterWidget->CharacterLevel->SetText(FText::FromString(FString::FromInt(Level)));
	}
}

