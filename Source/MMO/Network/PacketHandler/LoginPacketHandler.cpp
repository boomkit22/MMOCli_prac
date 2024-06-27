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
		UE_LOG(LogTemp, Warning, TEXT("PacketType default"));
		break;

	}

	CPacket::Free(packet);
}

void LoginPacketHandler::HandleLogin(CPacket* packet)
{
	//TODO : Login응답받은거니까



	if (UMMOGameInstance* GameInstance = UMMOGameInstance::GetInstance())
	{
		GameInstance->HandleLoginLogin(packet);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Handle Login GAme instacne null"));
	}
}

void LoginPacketHandler::HandleEcho(CPacket* packet)
{
	if (UMMOGameInstance* GameInstance = UMMOGameInstance::GetInstance())
	{
		GameInstance->HandleLoginEcho(packet);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("HHandleEcho Game instacne null"));
	}
}
