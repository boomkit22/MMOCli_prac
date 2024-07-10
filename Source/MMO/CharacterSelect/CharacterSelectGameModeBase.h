// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CharacterSelectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MMO_API ACharacterSelectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

private:
	void SetFixedCamera();
};
