// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GamePlayerController.h"

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AGamePlayerController::SpawnCharacter()
{
    if (GameCharacterClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        // 캐릭터 스폰
        AActor* SpawnedCharacter = GetWorld()->SpawnActor<AActor>(GameCharacterClass, SpawnParams);
        if (SpawnedCharacter)
        {
            Possess(Cast<APawn>(SpawnedCharacter));
        }
    }
}
