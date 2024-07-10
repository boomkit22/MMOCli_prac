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
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_FIELD_MOVE = 1004,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		PlayerInfo playerInfo
	//		SpawnLocation spawnLocation
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_SPAWN_MY_CHARACTER = 1005,


	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		PlayerInfo playerInfo
	//		SpawnLocation spawnLocation
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
	//		WORD	Type
	//		FVector Destination
	//      FVector StartRotation
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_CHARACTER_MOVE = 1009,


	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		int64 CharacterNO
	//		FVector Destination
	//		FVector StartRotation
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
	// 		int32   Damage
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
	//		int64   MonsterNO
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
	//		uint16  Class;
	//		TCHAR   yNickName[NICKNAME_LEN];
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_CREATE_PLAYER = 1023,



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