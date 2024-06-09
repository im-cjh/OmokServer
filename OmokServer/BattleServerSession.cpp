#include "pch.h"
#include "BattleServerSession.h"
#include "Protocol.pb.h"
#include "PacketHandler.h"

BattleServerSessionRef GBattleServer;

void BattleServerSession::Connect(INT16 port)
{
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // ���� ���� IP �ּ�
    serverAddr.sin_port = htons(port); // ��Ʋ ���� ��Ʈ

    if (::connect(_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        cout << "Connect error: " << err << endl;
        return;
    }
    else
    {
        cout << "Battle Server Connected!" << endl;
        Session::OnConnected();
    }

	
}

void BattleServerSession::MakeRoom(void)
{
    int len = 0;
    BYTE* sendBuffer = PacketHandler::SerializeMiniPacket(ePacketID::MAKE_FAST_ROOM_MESSAGE, &len);

    Session::Send(sendBuffer, len);
}

void BattleServerSession::HandlePacket(BYTE* buffer, INT32 len, ePacketID ID)
{
	switch (ID)
	{
	case MAKE_FAST_ROOM_MESSAGE:
        int a = 3;
        cout << a;
		break;
	}
}
