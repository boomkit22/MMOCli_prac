// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCharacterEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCharacterEntry::Init(ECharacterClassType characterClassType, FString level, FString id)
{
	switch (characterClassType)
	{
		case ECharacterClassType::CCT_Axe:
		{
			WeaponImage->SetBrushFromTexture(AxeImage);
		}
		break;

		case ECharacterClassType::CCT_Sword:
		{
			WeaponImage->SetBrushFromTexture(SwordImage);
		}
		break;
	}

	LevelText->SetText(FText::FromString(level));
	IDText->SetText(FText::FromString(id));
}
