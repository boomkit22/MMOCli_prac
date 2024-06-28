#include "Type.h"
#include "Network/DataStructure/SerializeBuffer.h"


CPacket& operator<<(CPacket& packet, FVector& vec)
{
	memcpy(&packet._buffer[packet._writePos], &vec, sizeof(double) * 3);
	packet._writePos += sizeof(FVector);
	packet._dataSize += sizeof(FVector);
	return packet;
}

CPacket& operator>>(CPacket& packet, FVector& vec)
{
	memcpy(&vec, &packet._buffer[packet._readPos], sizeof(FVector));
	packet._readPos += sizeof(FVector);
	packet._dataSize -= sizeof(FVector);
	return packet;
}

CPacket& operator<<(CPacket& packet, ResGameLoginInfo& resLoginInfo)
{
	int copySize = sizeof(int64) + sizeof(uint8) + sizeof(uint16) + sizeof(TCHAR) * ID_LEN;
	memcpy(&packet._buffer[packet._writePos], &resLoginInfo, copySize);
	packet._writePos += copySize;
	packet._dataSize += copySize;
	return packet;
	// TODO: ���⿡ return ���� �����մϴ�.
}

CPacket& operator>>(CPacket& packet, ResGameLoginInfo& resLoginInfo)
{
	int copySize = sizeof(int64) + sizeof(uint8) + sizeof(uint16) + sizeof(TCHAR) * ID_LEN;
	memcpy(&resLoginInfo, &packet._buffer[packet._readPos], copySize);
	packet._readPos += copySize;
	packet._dataSize -= copySize;
	return packet;
	// TODO: ���⿡ return ���� �����մϴ�.
}

CPacket& operator<<(CPacket& packet, SpawnMyCharacterInfo& spawnMyCharacterInfo)
{
	int copySize = sizeof(FVector) + sizeof(int64) + sizeof(uint16) + sizeof(TCHAR) * ID_LEN;
	memcpy(&packet._buffer[packet._writePos], &spawnMyCharacterInfo, copySize);
	packet._writePos += copySize;
	packet._dataSize += copySize;
	return packet;
}

CPacket& operator>>(CPacket& packet, SpawnMyCharacterInfo& spawnMyCharacterInfo)
{
	int copySize = sizeof(FVector) + sizeof(int64) + sizeof(uint16) + sizeof(TCHAR) * ID_LEN;
	memcpy(&spawnMyCharacterInfo, &packet._buffer[packet._readPos], copySize);
	packet._readPos += copySize;
	packet._dataSize -= copySize;
	return packet;
}

CPacket& operator<<(CPacket& packet, SpawnOtherCharacterInfo& spawnOtherCharacterInfo)
{
	int copySize = sizeof(FVector) + sizeof(int64) + sizeof(TCHAR) * ID_LEN + sizeof(uint16);
	memcpy(&packet._buffer[packet._writePos], &spawnOtherCharacterInfo, copySize);
	packet._writePos += copySize;
	packet._dataSize += copySize;
	return packet;
}

CPacket& operator>>(CPacket& packet, SpawnOtherCharacterInfo& spawnOtherCharacterInfo)
{
	int copySize = sizeof(FVector) + sizeof(int64) + sizeof(TCHAR) * ID_LEN + sizeof(uint16);
	memcpy(&spawnOtherCharacterInfo, &packet._buffer[packet._readPos], copySize);
	packet._readPos += copySize;
	packet._dataSize -= copySize;
	return packet;
}



