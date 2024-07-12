// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterSelectHUD.h"
#include "CharacterSelect/CharacterSelectOverlay.h"
#include "GameInstance/MMOGameInstance.h"

void ACharacterSelectHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = UMMOGameInstance::GetMMOWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && CharacterSelectOverlayClass)
		{
			// Mouse
			Controller->bShowMouseCursor = true;

			CharacterSelectOverlay = CreateWidget<UCharacterSelectOverlay>(Controller, CharacterSelectOverlayClass);
			if (CharacterSelectOverlay)
			{
				CharacterSelectOverlay->AddToViewport();
			}
		}
	}
}
