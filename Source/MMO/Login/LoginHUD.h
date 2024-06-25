// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoginHUD.generated.h"

/**
 * 
 */

class ULoginOverlay;
UCLASS()
class MMO_API ALoginHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "MMO")
	TSubclassOf<ULoginOverlay> LoginOverlayClass;

	UPROPERTY()
	ULoginOverlay* LoginOverlay;

public:
	FORCEINLINE ULoginOverlay* GetLoginOverlay() const { return LoginOverlay; }
};
