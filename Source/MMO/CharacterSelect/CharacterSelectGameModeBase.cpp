// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterSelectGameModeBase.h"
#include "CharacterSelect/CharacterSelectCamera.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/MMOGameInstance.h"

void ACharacterSelectGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetFixedCamera();
}

void ACharacterSelectGameModeBase::SetFixedCamera()
{
    // 모든 카메라 액터를 찾기?
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(UMMOGameInstance::GetMMOWorld(), ACharacterSelectCamera::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        // 흠 이거 그냥 0 하면 무조건 되는건가?
        ACharacterSelectCamera* FixedCameraActor = Cast<ACharacterSelectCamera>(FoundActors[0]);
        if (FixedCameraActor)
        {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(UMMOGameInstance::GetMMOWorld(), 0);
            if (PlayerController)
            {
                PlayerController->SetViewTargetWithBlend(FixedCameraActor);
            }
        }
    }
}
