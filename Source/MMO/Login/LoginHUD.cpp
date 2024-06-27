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
    // 현재 활성화된 오버레이가 있다면 제거
    if (CurrentOverlay)
    {
        CurrentOverlay->RemoveFromParent();
        CurrentOverlay = nullptr;
    }

    // 새 오버레이 생성 및 뷰포트에 추가
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