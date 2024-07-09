// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterSelectOverlay.h"

#include "Components/Button.h"
#include "CharacterSelect/CreateCharacterOverlay.h"

void UCharacterSelectOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	CreateButton->OnClicked.AddDynamic(this, &UCharacterSelectOverlay::OnCreateButtonClicked);
}

void UCharacterSelectOverlay::OnCreateButtonClicked()
{
	if (UCreateCharacterOverlayClass)
	{
		UCreateCharacterOverlay* CreateCharacterOverlay = CreateWidget<UCreateCharacterOverlay>(GetWorld(), UCreateCharacterOverlayClass);
		if (CreateCharacterOverlay)
		{
			CreateCharacterOverlay->AddToViewport();
		}
	}
}
