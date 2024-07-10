// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CreateCharacterOverlay.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "CharacterSelect/CharacterEntry.h"

void UCreateCharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	SwordClassButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnSwordClassButtonclicked);
	AxeClassButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnAxeClassButtonClicked);
    OkButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnOkButtonClicked);
}

void UCreateCharacterOverlay::SetCharacterSelectOverlay(UCharacterSelectOverlay* characterSelectOverlay)
{
	CharacterSelectOverlay = characterSelectOverlay;
}

void UCreateCharacterOverlay::OnSwordClassButtonclicked()
{
    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));

        FActorSpawnParameters SpawnParams;
        FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
        FVector spawnLocation = FVector(0.0f, 0.0f, 130.0f); // 예시 위치
        // 캐릭터 스폰
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetWorld()->GetName()));
        if (SpawnedCharacter != nullptr)
        {
            SpawnedCharacter->Destroy();
        }
        SpawnedCharacter = Cast<AGameCharacter>(GetWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, Rotation, SpawnParams));
        SpawnedCharacter->Initialize(ECharacterClassType::CCT_Sword);
        UE_LOG(LogTemp, Warning, TEXT("spawn2"));
        CharacterClassType = ECharacterClassType::CCT_Sword;
    }
}

void UCreateCharacterOverlay::OnAxeClassButtonClicked()
{
    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));
        FActorSpawnParameters SpawnParams;
        FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
        FVector spawnLocation = FVector(0.0f, 0.0f, 130.0f); // 예시 위치
        // 캐릭터 스폰
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetWorld()->GetName()));
        if (SpawnedCharacter != nullptr)
        {
            SpawnedCharacter->Destroy();
            SpawnedCharacter = nullptr;
        }
        SpawnedCharacter = Cast<AGameCharacter>(GetWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, Rotation, SpawnParams));
        SpawnedCharacter->Initialize(ECharacterClassType::CCT_Axe);
        UE_LOG(LogTemp, Warning, TEXT("spawn2"));
        CharacterClassType = ECharacterClassType::CCT_Axe;
    }
}

void UCreateCharacterOverlay::OnOkButtonClicked()
{
    if (SpawnedCharacter != nullptr)
	{
		SpawnedCharacter->Destroy();
		SpawnedCharacter = nullptr;
	}

    if(ECharacterClassType::CCT_None == CharacterClassType)
	{
        RemoveFromParent();
		return;
	}

   
    if (UCharacterEntryClass)
    {
        UCharacterEntry* CharacterEntry = CreateWidget<UCharacterEntry>(GetWorld(), UCharacterEntryClass);
        if (CharacterEntry)
        {
            CharacterEntry->Init(CharacterClassType, TEXT("1"), IDTextBox->GetText().ToString());
            if(CharacterSelectOverlay)
			{
				CharacterSelectOverlay->AddCharacterEntry(CharacterEntry);
			}
        }
    }
	RemoveFromParent();
}
