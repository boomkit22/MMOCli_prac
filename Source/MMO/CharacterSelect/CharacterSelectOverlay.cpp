// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterSelectOverlay.h"

#include "Components/Button.h"
#include "CharacterSelect/CreateCharacterOverlay.h"
#include "Components/VerticalBox.h"
#include "CharacterSelect/CharacterEntry.h"

void UCharacterSelectOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	CreateButton->OnClicked.AddDynamic(this, &UCharacterSelectOverlay::OnCreateButtonClicked);
}

void UCharacterSelectOverlay::AddCharacterEntry(UCharacterEntry* NewEntry)
{
	if (CharacterListVerticalBox && NewEntry)
	{
		CharacterListVerticalBox->AddChild(NewEntry);
	}
}

void UCharacterSelectOverlay::OnCreateButtonClicked()
{
	if (UCreateCharacterOverlayClass)
	{
		UCreateCharacterOverlay* CreateCharacterOverlay = CreateWidget<UCreateCharacterOverlay>(GetWorld(), UCreateCharacterOverlayClass);
		if (CreateCharacterOverlay)
		{
			CreateCharacterOverlay->AddToViewport();
			CreateCharacterOverlay->SetCharacterSelectOverlay(this);
		}
	}
}
