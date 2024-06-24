// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HUDMonsterComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API UHUDMonsterComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetHealthPercent(float Percent);
	void SetMonsterName(FString monsterName);

private:
	UPROPERTY()
	class UHUDMonsterWidget* MonsterWidget;
};

