// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MMOHUD.h"
#include "HUD/MMOOverlay.h"

void AMMOHUD::BeginPlay()
{
	Super::BeginPlay();


	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && MMOOverlayClass)
		{
			MMOOverlay = CreateWidget<UMMOOverlay>(Controller, MMOOverlayClass);
			if (MMOOverlay)
			{
				MMOOverlay->AddToViewport();
			}
		}
	}

}
