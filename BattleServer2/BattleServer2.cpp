#include "pch.h"
#include "PlayerListener.h"
#include "ServerListener.h"
#include "SocketUtils.h"
#include "Protocol.pb.h"
#include "PacketHeader.h"
#include "RoomManager.h"
#include "OmokServerSession.h"
#include "PacketHandler.h"

int main()
{
    SocketUtils::Init();
    GRoomManager.Init();
    PlayerListener playerListener(L"127.0.0.1", 8888);
    ServerListener serverListener(L"127.0.0.1", 8877);


    this_thread::sleep_for(1s);
    GOmokServer = make_shared<OmokServerSession>();
    GOmokServer->Connect(7788);

    for (int i = 0; i < 10; i += 1)
    {
        Protocol::S2CRoomID pkt;
        pkt.set_roomid(15);
        int len = 0;
        BYTE* sendBuffer = PacketHandler::SerializePacket(pkt, ePacketID::MAKE_FAST_ROOM_MESSAGE, &len);
        GOmokServer->Send(sendBuffer, len);
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

