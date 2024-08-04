// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

enum PACKET_TYPE
{
	//------------------------------------------------------
	// Game Server
	//------------------------------------------------------
	PACKET_GAME_SERVER = 1000,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	// 		TCHAR   ID[20]
	//		TCHAR	PassWord[20]     //����� PassWord. null����
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_LOGIN = 1001,



	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		int64   AccountNo;
	//		uint8	Status
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_LOGIN = 1002,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		uint16  fieldID
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_FIELD_MOVE = 1003,




	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		uint8  Status
	//		uin16 fieldID
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_FIELD_MOVE = 1004,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		PlayerInfo playerInfo
	//		FVector spawnLocation
	//		FRotator spawnRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_SPAWN_MY_CHARACTER = 1005,


	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		PlayerInfo playerInfo
	//		FRotator SpawnRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_SPAWN_OTHER_CHAACTER = 1006,

	//------------------------------------------------------------
	//	 �̰� �ʿ��Ѱ�
	//		WORD	Type
	//------------------------------------------------------------
	PACKET_SC_GAME_DESPAWN_MY_CHARACTER = 1007,


	//------------------------------------------------------------
	//	 
	//	WORD	Type
	//  int64 CharacterNO
	//------------------------------------------------------------
	PACKET_SC_GAME_DESPAWN_OTHER_CHARACTER = 1008,


	//------------------------------------------------------------
	// {
	//		WORD	 Type
	// 	    uint16   PathIndex;
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_CHARACTER_MOVE = 1009,


	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		int64 CharacterNO
	//		FVector Destination
	//		FRotator StartRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_CHARACTER_MOVE = 1010,

	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		int32   AttackerType
	//		int64   AttackerID
	//		int32   TargetType
	// 		int64   TargetID
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_CHARACTER_ATTACK = 1011,


	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		int32   AttackerType
	//		int64   AttackerID
	//		int32   TargetType
	// 		int64   TargetID
	// 		int32   Damage
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_DAMAGE = 1012,

	//------------------------------------------------------------
	// {
	//		WORD	Type
	//      FRotator StartRotation
	//		int32   SkillID
	//}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_CHARACTER_SKILL = 1013,

	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		int64   CharacterNO
	//		FRotator StartRotation
	//		int32   SkillID
	//
	// }
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_CHARACTER_SKILL = 1014,

	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		int64   MonsterID
	//		FVector StartPosition
	//		FRotator StartRotation
	//		int32   SkillID
	// }
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_MONSTER_SKILL = 1015,


	//------------------------------------------------------------
	//	{
	//		WORD	Type
	// 		TCHAR   ID[20]
	//		TCHAR	PassWord[20]     //����� PassWord. null����
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_SIGN_UP = 1016,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		uint8 Status
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_SIGN_UP = 1017,



	/*
	struct PlayerInfo
	{
		int64 PlayerID;
		TCHAR NickName[NICKNAME_LEN];
		uint16 Class;
		uint16 Level;
		uint32 Exp;
	};
	*/

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_PLAYER_LIST = 1018,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//      uint8   PlayerCount
	//		vector<PlayerInfo> PlayerInfos
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_PLAYER_LIST = 1019,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		int64   PlayerID
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_SELECT_PLAYER = 1020,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		uint8	Status
	//  �̰� ������ Field Move��û�ϰ�
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_SELECT_PLAYER = 1021,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		uint16 Class;
	//		TCHAR NickName[NICKNAME_LEN];
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_CREATE_PLAYER = 1022,


	//------------------------------------------------------------
	//	{
	//		WORD	Type
	// 		uint8	Status
	//		PlayerInfo playerInfo
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_CREATE_PLAYER = 1023,


	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		MonsterInfo monsterInfo
	//		SpawnLocation spawnLocation
	//		FRotator SpawnRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_SPAWN_MONSTER = 1024,


	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		int64		MonsterID
	//		FVector		Destination
	//		FRotator	StartRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_MONSTER_MOVE = 1025,


	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		FVector		StopLocation
	//		FRotator	StopRotation
	//	}
	//------------------------------------------------------------

	PACKET_CS_GAME_REQ_CHARACTER_STOP = 1026,

	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		int64		CharacterID
	//		FVector		StopLocation
	//		FRotator	StopRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_CHARACTER_STOP = 1027,


	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		int64		MonsterID
	//		FVector		StopLocation
	//		FRotator	StopRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_MONSTER_STOP = 1028,


	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		int64		CharacterID
	//		FVector		DeathLocation
	//		FRotator	DeathRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_CHARACTER_DEATH = 1029,

	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		int64		MonsterID
	//		FVector		DeathLocation
	// 		FRotator	DeathRotation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_MONSTER_DEATH = 1030,

	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		int64		MonsterID
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_DESPAWN_MONSTER = 1031,


	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		FVector		Destination
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_FIND_PATH = 1032,


	//------------------------------------------------------------
	// {
	//		WORD		Type
	//		uint16		PathCount
	//		vector<Pos>	Paths
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_FIND_PATH = 1033,


	//------------------------------------------------------------
	// ä�� ���� ��Ŷ
	//------------------------------------------------------------
	PACKET_CHATTING_SERVER = 5000,

	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		INT64	AccountNo           
	//		WCHAR	Nickname[20]		// null ����
	// }
	//------------------------------------------------------------
	PACKET_CS_CHAT_REQ_LOGIN = 5001,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		uint8	Status
	//	}
	//------------------------------------------------------------
	PACKET_SC_CHAT_RES_LOGIN = 5002,

	//------------------------------------------------------------
	// ä�ü��� ä�ú����� ��û
	//
	//	{
	//		WORD	Type
	//
	//		INT64	AccountNo
	//		WORD	MessageLen
	//		WCHAR	Message[MessageLen / 2]		// null ������
	//	}
	//
	//------------------------------------------------------------
	PACKET_CS_CHAT_REQ_MESSAGE = 5003,

	//------------------------------------------------------------
	// ä�ü��� ä�ú����� ����  (�ٸ� Ŭ�� ���� ä�õ� �̰ɷ� ����)
	//
	//	{
	//		WORD	Type												
	//
	//		INT64	AccountNo
	//		WCHAR	Nickname[20]				// null ����
	//		
	//		WORD	MessageLen
	//		WCHAR	Message[MessageLen / 2]		// null ������
	//	}
	//
	//------------------------------------------------------------
	PACKET_SC_CHAT_RES_MESSAGE = 5004,
};