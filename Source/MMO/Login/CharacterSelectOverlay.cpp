// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/CharacterSelectOverlay.h"
#include "Components/Button.h"

void UCharacterSelectOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	CharacterSelectButton->OnClicked.AddDynamic(this, &UCharacterSelectOverlay::OnCharacterSelectButtonClicked);
}

void UCharacterSelectOverlay::OnCharacterSelectButtonClicked()
{
	// �̰� Ŭ���ϸ� ���ؾ��ϴµ�?
	// TODO: �ʵ��̵� ��û
	
	// 1. Req Field Move

	// 2. Wait For Response
	// 2. 1 Res FiledMove
	// 2. 2 Spawn My Character
	// 2. 3 Spanw Other Chater
	// 2. 4 Spawn Monster
}
