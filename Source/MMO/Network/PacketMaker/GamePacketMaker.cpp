// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketMaker/GamePacketMaker.h"
#include "Network/PacketHeader.h"
#include "DataStructure/SerializeBuffer.h"
#include "Network/Packet.h"

void GamePacketMaker::MP_CS_REQ_LOGIN(CPacket* Packet, int64& accountNo)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	Packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_LOGIN;

	*Packet << type << accountNo;

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}
