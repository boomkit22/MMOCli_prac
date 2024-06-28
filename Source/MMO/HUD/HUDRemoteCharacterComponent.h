// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HUDRemoteCharacterComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UHUDRemoteCharacterComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent);
	void SetCharacterName(FString Name);
	void SetCharacterLevel(uint16 Level);

private:
	UPROPERTY()
	class UHUDRemoteCharacterWidget* RemoteCharacterWidget;
};
