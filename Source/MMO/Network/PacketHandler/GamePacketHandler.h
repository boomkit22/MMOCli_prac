// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "GameInstance/MMOGameInstance.h"
#include "CoreMinimal.h"
/**
 * 
 */

class CPacket;
class MMO_API GamePacketHandler
{
public:
	static void HandlePacket(CPacket* packet);

private:
	inline static UMMOGameInstance* GameInstance = nullptr;
};
