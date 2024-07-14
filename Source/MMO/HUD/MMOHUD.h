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
class UMMOToLobbyOverlay;

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

	UPROPERTY(EditDefaultsOnly, Category = "MMO")
	TSubclassOf<UMMOToLobbyOverlay> MMOToLobbyOverlayClass;


	UPROPERTY()
	UMMOOverlay* MMOOverlay;

	UPROPERTY()
	UMMOChatOverlay* MMOChatOverlay;

	UPROPERTY()
	UMMOToLobbyOverlay* MMOToLobbyOverlay;

public:
	FORCEINLINE UMMOOverlay* GetMMOOverlay() const { return MMOOverlay; }
	FORCEINLINE UMMOChatOverlay* GetMMOChatOverlay() const { return MMOChatOverlay; }
};
