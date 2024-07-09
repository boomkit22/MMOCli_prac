// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterSelectHUD.h"
#include "CharacterSelect/CharacterSelectOverlay.h"

void ACharacterSelectHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && CharacterSelectOverlayClass)
		{
			CharacterSelectOverlay = CreateWidget<UCharacterSelectOverlay>(Controller, CharacterSelectOverlayClass);
			if (CharacterSelectOverlay)
			{
				CharacterSelectOverlay->AddToViewport();
			}
		}
	}
}
