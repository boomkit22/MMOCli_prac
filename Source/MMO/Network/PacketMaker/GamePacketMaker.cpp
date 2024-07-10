// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketMaker/GamePacketMaker.h"
#include "Network/PacketHeader.h"
#include "DataStructure/SerializeBuffer.h"
#include "Network/Packet.h"

void GamePacketMaker::MP_CS_REQ_LOGIN(CPacket* Packet, TCHAR* id, TCHAR* passWord)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	Packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_LOGIN;

	*Packet << type;
	Packet->PutData((char*)id, sizeof(TCHAR) * ID_LEN);
	Packet->PutData((char*)passWord, sizeof(TCHAR) * PASSWORD_LEN);

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_CS_REQ_FIELD_MOVE(CPacket* Packet, uint16& fieldID)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	Packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_FIELD_MOVE;

	*Packet << type << fieldID;

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_CS_REQ_CHARACTER_MOVE(CPacket* Packet, FVector& Destination, FRotator& StartRotation)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	Packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_CHARACTER_MOVE;

	*Packet << type << Destination << StartRotation;

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_CS_REQ_CHARACTER_ATTACK(CPacket* packet, int32 AttackerType, int64 AttackerID, int32 TargetType, int64 TargetID, int32 Damage)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_CHARACTER_ATTACK;

	*packet << type << AttackerType << AttackerID << TargetType << TargetID << Damage;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_SC_REQ_CHARACTER_SKILL(CPacket* packet, FRotator& StartRotator, int32 SkillID)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_CHARACTER_SKILL;

	*packet << type << StartRotator << SkillID;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_CS_REQ_SIGN_UP(CPacket* Packet, TCHAR* id, TCHAR* passWord)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	Packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_SIGN_UP;

	*Packet << type;
	Packet->PutData((char*)id, sizeof(TCHAR) * ID_LEN);
	Packet->PutData((char*)passWord, sizeof(TCHAR) * PASSWORD_LEN);

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_CS_REQ_PLAYER_LIST(CPacket* packet)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_PLAYER_LIST;

	*packet << type;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_CS_REQ_CREATE_PLAYER(CPacket* packet, uint16 Class, TCHAR* NickName)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_CREATE_PLAYER;

	*packet << type << Class;
	packet->PutData((char*)NickName, sizeof(TCHAR) * NICKNAME_LEN);

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void GamePacketMaker::MP_CS_REQ_SELECT_PLAYER(CPacket* packet, int64 PlayerID)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_GAME_REQ_SELECT_PLAYER;

	*packet << type << PlayerID;

	uint16 len = (uint16)(packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}


