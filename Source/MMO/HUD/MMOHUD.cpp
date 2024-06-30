// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MMOHUD.h"
#include "HUD/MMOOverlay.h"
#include "HUD/MMOChatOverlay.h"

void AMMOHUD::BeginPlay()
{
	Super::BeginPlay();


	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && MMOOverlayClass && MMOChatOverlayClass)
		{
			MMOOverlay = CreateWidget<UMMOOverlay>(Controller, MMOOverlayClass);
			if (MMOOverlay)
			{
				MMOOverlay->AddToViewport();
			}

			MMOChatOverlay = CreateWidget<UMMOChatOverlay>(Controller, MMOChatOverlayClass);
			if (MMOChatOverlay)
			{
				MMOChatOverlay->AddToViewport();
			}
		}
	}

}
