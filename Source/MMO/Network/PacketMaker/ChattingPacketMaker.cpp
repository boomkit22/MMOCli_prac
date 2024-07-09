// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketMaker/ChattingPacketMaker.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "Network/Packet.h"

void ChattingPacketMaker::MP_CS_REQ_LOGIN(CPacket* Packet, int64& AccountNo, TCHAR* NickName)
{
	NetHeader Header;
	Header._code = GamePacketCode;
	Header._randKey = rand();

	Packet->PutData((char*)&Header, sizeof(NetHeader));
	uint16 type = PACKET_CS_CHAT_REQ_LOGIN;
	*Packet << type << AccountNo;
	
    Packet->PutData((char*)NickName, NICKNAME_LEN * sizeof(TCHAR));

	uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
	FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}

void ChattingPacketMaker::MP_CS_REQ_MESSAGE(CPacket* Packet, int64& accountNo, FString& message)
{
    // 패킷 헤더 설정
    NetHeader Header;
    Header._code = GamePacketCode; // 게임 패킷 코드, 실제 값으로 대체 필요
    Header._randKey = rand(); // 랜덤 키 생성

    Packet->PutData((char*)&Header, sizeof(NetHeader));

    // 패킷 타입 설정
    uint16 type = PACKET_CS_CHAT_REQ_MESSAGE; // 실제 패킷 타입 코드로 대체 필요
    *Packet << type << accountNo;

    // 메시지 길이 계산 (WCHAR 기준, null 미포함)
    uint16 messageLen = message.Len() * sizeof(TCHAR);
    *Packet << messageLen;

    // FString을 WCHAR 배열로 변환하여 패킷에 추가
    if (messageLen > 0)
    {
        TCHAR* messageArray = message.GetCharArray().GetData();
        Packet->PutData((char*)messageArray, messageLen);
    }

    // 패킷 전체 길이 업데이트
    uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
    FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}