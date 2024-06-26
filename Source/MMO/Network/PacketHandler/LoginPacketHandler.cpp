// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHandler/LoginPacketHandler.h"
#include "DataStructure/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"
#include "Packet.h"


void LoginPacketHandler::HandlePacket(CPacket* packet)
{
	int16 packetType;
	*packet >> packetType;

	switch (packetType)
	{
	case PACKET_SC_LOGIN_RES_LOGIN:
	{
		HandleLogin(packet);
	}
	break;

	case PACKET_SC_LOGIN_RES_ECHO:
	{
		HandleEcho(packet);
	}
	break;

	default:
		break;

	}

	CPacket::Free(packet);
}

void LoginPacketHandler::HandleLogin(CPacket* packet)
{
	//TODO : Login��������Ŵϱ�

	if (auto* GameInstance = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLogin(packet);
	}
}

void LoginPacketHandler::HandleEcho(CPacket* packet)
{
	if (auto* GameInstance = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleEcho(packet);
	}
}
