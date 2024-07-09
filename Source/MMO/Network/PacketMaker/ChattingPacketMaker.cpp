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
    // ��Ŷ ��� ����
    NetHeader Header;
    Header._code = GamePacketCode; // ���� ��Ŷ �ڵ�, ���� ������ ��ü �ʿ�
    Header._randKey = rand(); // ���� Ű ����

    Packet->PutData((char*)&Header, sizeof(NetHeader));

    // ��Ŷ Ÿ�� ����
    uint16 type = PACKET_CS_CHAT_REQ_MESSAGE; // ���� ��Ŷ Ÿ�� �ڵ�� ��ü �ʿ�
    *Packet << type << accountNo;

    // �޽��� ���� ��� (WCHAR ����, null ������)
    uint16 messageLen = message.Len() * sizeof(TCHAR);
    *Packet << messageLen;

    // FString�� WCHAR �迭�� ��ȯ�Ͽ� ��Ŷ�� �߰�
    if (messageLen > 0)
    {
        TCHAR* messageArray = message.GetCharArray().GetData();
        Packet->PutData((char*)messageArray, messageLen);
    }

    // ��Ŷ ��ü ���� ������Ʈ
    uint16 len = (uint16)(Packet->GetDataSize() - sizeof(NetHeader));
    FMemory::Memcpy(Packet->GetBufferPtr() + NET_HEADER_SIZE_INDEX, (void*)&len, sizeof(uint16));
}