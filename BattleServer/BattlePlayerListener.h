#pragma once
#include "Listener.h"
#include "BattleServerPlayer.h"
#include <functional>

class BattlePlayerListener : public Listener
{
public:
    BattlePlayerListener(wstring ip, INT16 port) : Listener(ip, port, []() { return make_shared<BattleServerPlayer>(); })
    {

    }
};

