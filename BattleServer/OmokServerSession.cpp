#include "pch.h"
#include "OmokServerSession.h"
#include "RoomManager.h"

OmokServerSessionRef GOmokServer;

void OmokServerSession::Connect(INT16 port)
{
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // ���� ���� IP �ּ�
    serverAddr.sin_port = htons(port); // ���� ���� ��Ʈ

    if (::connect(_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        cout << "Connect error: " << err << endl;
        return;
    }
    else
    {
        cout << " OmokServer Connected!" << endl;
        Session::OnConnected();
    }
    
}

void OmokServerSession::HandlePacket(BYTE* pBuffer, INT32 pLen, ePacketID ID)
{
	switch (ID)
	{
	case MAKE_FAST_ROOM_MESSAGE: 
        GRoomManager.HandleMakeFastRoom(pBuffer, pLen);
		break;
	default:
		break;
	}
}

