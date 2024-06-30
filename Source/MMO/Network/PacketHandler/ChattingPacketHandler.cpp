// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHandler/ChattingPacketHandler.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "Type.h"
#include "Network/Packet.h"
#include "GameInstance/MMOGameInstance.h"


void ChattingPacketHandler::HandlePacket(CPacket* packet)
{
	int16 packetType;
	*packet >> packetType;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[Chatting]Handle Packet Type : %d"), packetType));

	switch (packetType)
	{
	case PACKET_SC_CHAT_RES_LOGIN:
	{
		HandleLogin(packet);
	}
	break;

	case PACKET_SC_CHAT_RES_MESSAGE:
	{
		HandleMessage(packet);
	}
	break;



	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Unknown Packet Type : %d"), packetType));
		break;
	}

	CPacket::Free(packet);

}

void ChattingPacketHandler::HandleLogin(CPacket* packet)
{
	int64 AccountNo;
	uint8 Status;
	*packet >> AccountNo >> Status;

	if (Status == false)
	{
		// 실패면?
		// TODO: 게임종료
	}
}

void ChattingPacketHandler::HandleMessage(CPacket* packet)
{
	if (UMMOGameInstance* GameInstance = UMMOGameInstance::GetInstance())
	{
		GameInstance->HandleChatMessage(packet);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Handle Login GAme instacne null"));
	}
}
