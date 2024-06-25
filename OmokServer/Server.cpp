#include "pch.h"
#include "SocketUtils.h"
#include "Protocol.pb.h"
#include "PacketHeader.h"
#include "RoomManager.h"
#include "Protocol.pb.h"
#include "BattleServerSession.h"
#include "PacketHandler.h"
#include "Player.h"
#include "PlayerListener.h"
#include "ServerListener.h"

int main()
{
    SocketUtils::Init();
    GRoomManager.Init();
    PlayerListener playerListener(L"127.0.0.1", 7777);
    ServerListener serverListener(L"127.0.0.1", 7788);

    GBattleServer = make_shared<BattleServerSession>();
    GBattleServer->Connect(8877);

    
    for (int i = 0; i < 1; i += 1)
    {
        Protocol::S2CRoomID pkt;
        pkt.set_roomid(15);
        int len = 0;
        BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::MAKE_FAST_ROOM_MESSAGE, &len);
        GBattleServer->Send(sendBuffer, len);
    }


    vector<thread> workerThreads;

    for (int i = 0; i < 5; ++i)
    {
        workerThreads.push_back(thread([=]() {
            while (true)
                GIocpCore.Dispatch();
            }));
    }

    thread tPlayerListener(&Listener::StartAccept, &playerListener);
    thread tServerListener(&Listener::StartAccept, &serverListener);

    // 모든 스레드가 종료될 때까지 대기
    for (auto& workerThread : workerThreads)
        workerThread.join();
    tPlayerListener.join();
    tServerListener.join();

    return 0;
}

