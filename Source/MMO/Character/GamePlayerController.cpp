// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GamePlayerController.h"
#include "Character/GameCharacter.h"
#include "HUD/MMOHUD.h"
#include "HUD/MMOOverlay.h"
void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

}

AGameCharacter* AGamePlayerController::SpawnMyCharacter(FVector spawnLocation, PlayerInfo playerInfo)
{
    AGameCharacter* GameCharacter = nullptr;
    UE_LOG(LogTemp, Warning, TEXT("spawn3"));

    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));

        FActorSpawnParameters SpawnParams;
        FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
        SpawnParams.Owner = this;
        // 캐릭터 스폰
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetWorld()->GetName()));
        AActor* SpawnedCharacter = GetWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, Rotation, SpawnParams);
        if (SpawnedCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("spawn1"));
            Possess(Cast<APawn>(SpawnedCharacter));

            GameCharacter = Cast<AGameCharacter>(SpawnedCharacter);
            if (GameCharacter)
            {
                FString name = playerInfo.NickName;
                uint16 level = playerInfo.Level;

                GameCharacter->Initialize(static_cast<ECharacterClassType>(playerInfo.Class));
                GameCharacter->SetPlayerID(playerInfo.PlayerID);
				GameCharacter->InitCharAttributeComponent(100, name, level);
                
                AMMOHUD* MMOHUD = Cast<AMMOHUD>(GetHUD());
                if (MMOHUD)
                {
                    UMMOOverlay* MMOOverlay = MMOHUD->GetMMOOverlay();
                    if (MMOOverlay)
                    {
                        MMOOverlay->SetHealthBarPercent(1);
                        MMOOverlay->SetStaminaBarPercent(1);
                        MMOOverlay->SetExperienceBarPercent(0);
                        MMOOverlay->SetLevelTextBlock(level);
                    }
                }

            }
        }
        UE_LOG(LogTemp, Warning, TEXT("spawn2"));
    }

    return GameCharacter;
}
