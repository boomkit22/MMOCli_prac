// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientSession.h"

/**
 * 
 */
class MMO_API LoginServerSession : public ClientSession
{
public:
	LoginServerSession();
	~LoginServerSession();

	void HandleRecvPacket() override;
};
