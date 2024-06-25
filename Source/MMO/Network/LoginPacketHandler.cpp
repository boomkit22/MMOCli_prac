// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/LoginPacketHandler.h"
#include "Network/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"


void LoginPacketHandler::HandlePacket(CPacket* packet)
{
	int16 packetType;
	*packet >> packetType;

	switch (packetType)
	{
	case 1:
		Handle_Login(packet);
		break;
	}
}

void LoginPacketHandler::Handle_Login(CPacket* packet)
{
	//TODO : Login응답받은거니까

	if (auto* GameInstance = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLogin(packet);
	}
}
