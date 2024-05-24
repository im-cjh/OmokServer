#pragma once
#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <Windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <memory>
#include <iostream>
#include <vector>
#include <set>
#include <atomic>
#include <array>
#include <queue>
#include <mutex>
#include <map>
#include <unordered_map>
#include "Types.h"
#include "Macro.h"
#include "PacketHeader.h"

using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif