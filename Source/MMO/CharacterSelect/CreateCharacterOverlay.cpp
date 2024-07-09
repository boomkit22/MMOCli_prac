// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CreateCharacterOverlay.h"
#include "Components/Button.h"
void UCreateCharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	SwordClassButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnSwordClassButtonclicked);
	AxeClassButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnAxeClassButtonClicked);
}

void UCreateCharacterOverlay::OnSwordClassButtonclicked()
{
    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));

        FActorSpawnParameters SpawnParams;
        FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
        FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f); // 예시 위치
        // 캐릭터 스폰
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetWorld()->GetName()));
        AActor* SpawnedCharacter = GetWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, Rotation, SpawnParams);
        UE_LOG(LogTemp, Warning, TEXT("spawn2"));
    }
}

void UCreateCharacterOverlay::OnAxeClassButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("AxeClassButton Clicked"));
}
