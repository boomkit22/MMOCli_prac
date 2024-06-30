// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MMOHUD.generated.h"

/**
 * 
 */
class UMMOChatOverlay;
class UMMOOverlay;

UCLASS()
class MMO_API AMMOHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "MMO")
	TSubclassOf<UMMOOverlay> MMOOverlayClass;

	UPROPERTY(EditDefaultsOnly, Category = "MMO")
	TSubclassOf<UMMOChatOverlay> MMOChatOverlayClass;

	UPROPERTY()
	UMMOOverlay* MMOOverlay;

	UPROPERTY()
	UMMOChatOverlay* MMOChatOverlay;

public:
	FORCEINLINE UMMOOverlay* GetMMOOverlay() const { return MMOOverlay; }
};
