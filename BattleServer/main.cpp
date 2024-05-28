#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "Protocol.pb.h"
#include "PacketHeader.h"
#include "RoomManager.h"

int main()
{
    SocketUtils::Init();
    GRoomManager.Init();
    Listener listener(L"127.0.0.1", 8888);
    //Listener listener(L"203.237.81.67", 7777);

    vector<thread> workerThreads;

    for (int i = 0; i < 5; ++i)
    {
        workerThreads.push_back(thread([=]() {
            while (true)
                GIocpCore.Dispatch();
            }));
    }

    thread tListener(&Listener::StartAccept, &listener);

    // ��� �����尡 ����� ������ ���
    for (auto& workerThread : workerThreads)
        workerThread.join();
    tListener.join();

    return 0;
}
