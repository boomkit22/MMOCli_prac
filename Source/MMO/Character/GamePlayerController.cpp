// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GamePlayerController.h"

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AGamePlayerController::SpawnMyCharacter(FVector spawnLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("spawn3"));

    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));

        FActorSpawnParameters SpawnParams;
        FVector l{ 100, 100, 100 };
        FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
        SpawnParams.Owner = this;
        // 캐릭터 스폰
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetWorld()->GetName()));
        AActor* SpawnedCharacter = GetWorld()->SpawnActor<AActor>(GameCharacterClass, l, Rotation, SpawnParams);
        if (SpawnedCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("spawn1"));
            Possess(Cast<APawn>(SpawnedCharacter));
        }
        UE_LOG(LogTemp, Warning, TEXT("spawn2"));

    }
}
