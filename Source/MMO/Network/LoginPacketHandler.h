// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CPacket;

class MMO_API LoginPacketHandler
{
public:
	static void HandlePacket(CPacket* packet);
	static void Handle_Login(CPacket* packet);
};
