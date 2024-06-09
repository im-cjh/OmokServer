#include "pch.h"
#include "OmokServerSession.h"

OmokServerSessionRef GOmokServer;

void OmokServerSession::Connect(INT16 port)
{
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 오목 서버 IP 주소
    serverAddr.sin_port = htons(port); // 오목 서버 포트

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

void OmokServerSession::HandlePacket(BYTE* buffer, INT32 len, ePacketID ID)
{
	switch (ID)
	{
	case MAKE_FAST_ROOM_MESSAGE:
    {
        int a = 23;
        cout << a;
    }
		break;
	default:
		break;
	}
}

void OmokServerSession::HandleMakeFastRoom(BYTE* buffer, INT32 len)
{

}
