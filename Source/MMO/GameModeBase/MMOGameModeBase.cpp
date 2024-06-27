// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/MMOGameModeBase.h"
#include "Character/GamePlayerController.h"

AMMOGameModeBase::AMMOGameModeBase()
{
    // Set default pawn class to none
    DefaultPawnClass = nullptr;
    
    // Use custom PlayerController class
    PlayerControllerClass = AGamePlayerController::StaticClass();
}
