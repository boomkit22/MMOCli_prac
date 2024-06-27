// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketMaker/LoginPacketMaker.h"
#include "Network/PacketHeader.h"
#include "DataStructure/SerializeBuffer.h"
#include "Network/Packet.h"

void LoginPacketMaker::MP_CS_REQ_LOGIN(CPacket* Packet, TCHAR* Id, TCHAR* PassWord)
{
	NetHeader Header;
	Header._code = LoginPacketCode;
	Header._randKey = rand();
	
	Packet->PutData((char*)&Header, sizeof(NetHeader));

	uint16 type = PACKET_CS_LOGIN_REQ_LOGIN;
	*Packet << type;
	Packet->PutData((char*)Id, dfID_LEN * sizeof(TCHAR));
	Packet->PutData((char*)PassWord, dfPASSWORD_LEN * sizeof(TCHAR));

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));

}

void LoginPacketMaker::MP_CS_REQ_ECHO(CPacket* Packet)
{
	NetHeader Header;
	Header._code = LoginPacketCode;
	Header._randKey = rand();

	Packet->PutData((char*)&Header, sizeof(NetHeader));

	uint16 type = PACKET_CS_LOGIN_REQ_ECHO;
	*Packet << type;

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}
