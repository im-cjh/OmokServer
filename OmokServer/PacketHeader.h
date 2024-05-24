#pragma once

//Server �ڵ�
enum ePacketID  : UINT16
{
	CHAT_MESSAGE = 0,
	ROOMS_MESSAGE = 1,
	CONTENT_MESSAGE = 2,
	ENTER_ROOM_MESSAGE = 3,
	LOGIN_SUCCESS_MESSAGE = 4,
	WINNER_MESSAGE = 5,
	QUIT_ROOM_MESSAGE = 6,
	MATCHMAKIING_MESSAGE=7
};

struct PacketHeader
{
	UINT16 size;
	UINT16 id; // ��������ID (ex. 1=�α���, 2=�̵���û)
};