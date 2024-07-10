// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterSelectHUD.generated.h"

/**
 * 
 */

class UCharacterSelectOverlay;

UCLASS()
class MMO_API ACharacterSelectHUD : public AHUD
{
	GENERATED_BODY()

public:
	class UCharacterSelectOverlay* GetCharacterSelectOverlay() const { return CharacterSelectOverlay; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UCharacterSelectOverlay> CharacterSelectOverlayClass;

	UPROPERTY()
	UCharacterSelectOverlay* CharacterSelectOverlay;

};
