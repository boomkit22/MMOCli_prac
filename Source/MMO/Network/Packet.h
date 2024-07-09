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
	//		TCHAR	PassWord[20]     //사용자 PassWord. null포함
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_LOGIN = 1001,


	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		int64   AccountNo;
	//		uint8	Status
	//		uint16  CharacterLevel
	//	    TCHAR   NickName[20] // null포함
	//		uint32  Exp
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
	// 		int64 PlayerID // 한 계정에 플레이어가 여러개 있어도 PlayerID는 고유하다
	//		FVector SpawnLocation
	// 		uint16 Level
	// 		TCHAR NickName[20] // null포함
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_SPAWN_MY_CHARACTER = 1005,


	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		int64 PlayerID // 한 계정에 플레이어가 여러개 있어도 PlayerID는 고유하다
	// 		FVector SpawnLocation
	// 		uint16 Level
	// 		TCHAR NickName[20] // null포함
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_SPAWN_OTHER_CHAACTER = 1006,

	//------------------------------------------------------------
	//	 이거 필요한가
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
	//		TCHAR	PassWord[20]     //사용자 PassWord. null포함
	//	}
	//------------------------------------------------------------
	PACKET_CS_GAME_REQ_SIGN_UP = 1016,

	//------------------------------------------------------------
	//	{
	//		WORD	Type
	//		int64 Status
	//	}
	//------------------------------------------------------------
	PACKET_SC_GAME_RES_SIGN_UP = 1017,

	//------------------------------------------------------------
	// 채팅 서버 패킷
	//------------------------------------------------------------
	PACKET_CHATTING_SERVER = 5000,

	//------------------------------------------------------------
	// {
	//		WORD	Type
	//		INT64	AccountNo           
	//		WCHAR	Nickname[20]		// null 포함
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
	// 채팅서버 채팅보내기 요청
	//
	//	{
	//		WORD	Type
	//
	//		INT64	AccountNo
	//		WORD	MessageLen
	//		WCHAR	Message[MessageLen / 2]		// null 미포함
	//	}
	//
	//------------------------------------------------------------
	PACKET_CS_CHAT_REQ_MESSAGE = 5003,

	//------------------------------------------------------------
	// 채팅서버 채팅보내기 응답  (다른 클라가 보낸 채팅도 이걸로 받음)
	//
	//	{
	//		WORD	Type
	//
	//		INT64	AccountNo
	//		WCHAR	Nickname[20]				// null 포함
	//		
	//		WORD	MessageLen
	//		WCHAR	Message[MessageLen / 2]		// null 미포함
	//	}
	//
	//------------------------------------------------------------
	PACKET_SC_CHAT_RES_MESSAGE = 5004,
};