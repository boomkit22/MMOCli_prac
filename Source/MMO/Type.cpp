#include "Type.h"
#include "Network/DataStructure/SerializeBuffer.h"

CPacket& operator<<(CPacket& packet, FRotator& rot)
{
	packet << rot.Pitch << rot.Yaw << rot.Roll;
	return packet;
}

CPacket& operator>>(CPacket& packet, FRotator& rot)
{
	packet >> rot.Pitch >> rot.Yaw >> rot.Roll;
	return packet;
}

CPacket& operator<<(CPacket& packet, FVector& vec)
{
	packet << vec.X << vec.Y << vec.Z;
	return packet;
}

CPacket& operator>>(CPacket& packet, FVector& vec)
{
	packet >> vec.X >> vec.Y >> vec.Z;
	return packet;
}

CPacket& operator<<(CPacket& packet, ResGameLoginInfo& resLoginInfo)
{
	packet << resLoginInfo.AccountNo << resLoginInfo.Status << resLoginInfo.Level;
	packet.PutData((char*)resLoginInfo.NickName, sizeof(TCHAR) * ID_LEN);
	return packet;
}

CPacket& operator>>(CPacket& packet, ResGameLoginInfo& resLoginInfo)
{
	packet >> resLoginInfo.AccountNo >> resLoginInfo.Status >> resLoginInfo.Level;
	packet.GetData((char*)resLoginInfo.NickName, sizeof(TCHAR) * ID_LEN);
	return packet;
	// TODO: ���⿡ return ���� �����մϴ�.
}

CPacket& operator<<(CPacket& packet, SpawnMyCharacterInfo& spawnMyCharacterInfo)
{
	packet << spawnMyCharacterInfo.SpawnLocation << spawnMyCharacterInfo.PlayerID << spawnMyCharacterInfo.Level;
	packet.PutData((char*)spawnMyCharacterInfo.NickName, sizeof(TCHAR) * ID_LEN);
	return packet;
}

CPacket& operator>>(CPacket& packet, SpawnMyCharacterInfo& spawnMyCharacterInfo)
{
	packet >> spawnMyCharacterInfo.SpawnLocation >> spawnMyCharacterInfo.PlayerID >> spawnMyCharacterInfo.Level;
	packet.GetData((char*)spawnMyCharacterInfo.NickName, sizeof(TCHAR) * ID_LEN);
	return packet;
}

CPacket& operator<<(CPacket& packet, SpawnOtherCharacterInfo& spawnOtherCharacterInfo)
{
	packet << spawnOtherCharacterInfo.SpawnLocation << spawnOtherCharacterInfo.PlayerID << spawnOtherCharacterInfo.Level;
	packet.PutData((char*)spawnOtherCharacterInfo.NickName, sizeof(TCHAR) * ID_LEN);
	return packet;
}

CPacket& operator>>(CPacket& packet, SpawnOtherCharacterInfo& spawnOtherCharacterInfo)
{
	packet >> spawnOtherCharacterInfo.SpawnLocation >> spawnOtherCharacterInfo.PlayerID >> spawnOtherCharacterInfo.Level;
	packet.GetData((char*)spawnOtherCharacterInfo.NickName, sizeof(TCHAR) * ID_LEN);
	return packet;
}


