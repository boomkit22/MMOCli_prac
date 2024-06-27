// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/LoginHUD.h"
#include "Login/LoginOverlay.h"
#include "Login/CharacterSelectOverlay.h"

void ALoginHUD::BeginPlay()
{
	Super::BeginPlay();
    ChangeOverlay(LoginOverlayClass);
}

void ALoginHUD::ChangeOverlay(TSubclassOf<UUserWidget> NewOverlayClass)
{
    // ���� Ȱ��ȭ�� �������̰� �ִٸ� ����
    if (CurrentOverlay)
    {
        CurrentOverlay->RemoveFromParent();
        CurrentOverlay = nullptr;
    }

    // �� �������� ���� �� ����Ʈ�� �߰�
    UWorld* World = GetWorld();
    if (World)
    {
        APlayerController* Controller = World->GetFirstPlayerController();
        Controller->bShowMouseCursor = true;
        if (Controller && NewOverlayClass)
        {
            CurrentOverlay = CreateWidget<UUserWidget>(Controller, NewOverlayClass);
            if (CurrentOverlay)
            {
                CurrentOverlay->AddToViewport();
            }
        }
    }
}