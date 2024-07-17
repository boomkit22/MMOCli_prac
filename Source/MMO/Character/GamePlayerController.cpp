// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GamePlayerController.h"
#include "Character/GameCharacter.h"
#include "HUD/MMOHUD.h"
#include "HUD/MMOOverlay.h"
#include "GameInstance/MMOGameInstance.h"

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

}

AGameCharacter* AGamePlayerController::SpawnMyCharacter(FVector spawnLocation, PlayerInfo playerInfo, FRotator SpawnRoation)
{
    AGameCharacter* GameCharacter = nullptr;
    UE_LOG(LogTemp, Warning, TEXT("spawn3"));

    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        // 캐릭터 스폰
        AActor* SpawnedCharacter = UMMOGameInstance::GetMMOWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, SpawnRoation, SpawnParams);
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
				GameCharacter->InitCharAttributeComponent(playerInfo.Hp, name, level);
                
                AMMOHUD* MMOHUD = Cast<AMMOHUD>(GetHUD());
                if (MMOHUD)
                {
                    UMMOOverlay* MMOOverlay = MMOHUD->GetMMOOverlay();
                    if (MMOOverlay)
                    {
                        MMOOverlay->SetHealthBarPercent(playerInfo.Hp / 100.f);
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
