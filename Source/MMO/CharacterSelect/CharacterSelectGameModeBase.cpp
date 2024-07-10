// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterSelectGameModeBase.h"
#include "CharacterSelect/CharacterSelectCamera.h"
#include "Kismet/GameplayStatics.h"

void ACharacterSelectGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetFixedCamera();
}

void ACharacterSelectGameModeBase::SetFixedCamera()
{
    // ��� ī�޶� ���͸� ã���ϴ�.
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterSelectCamera::StaticClass(), FoundActors);

    if (FoundActors.Num() > 0)
    {
        // ù ��° ī�޶� ���͸� ������ ī�޶�� ����մϴ�.
        ACharacterSelectCamera* FixedCameraActor = Cast<ACharacterSelectCamera>(FoundActors[0]);
        if (FixedCameraActor)
        {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PlayerController)
            {
                PlayerController->SetViewTargetWithBlend(FixedCameraActor);
            }
        }
    }
}
