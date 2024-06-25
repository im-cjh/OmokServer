#pragma once
#include "Listener.h"
#include <functional>
#include "Player.h"

class PlayerListener : public Listener
{
public:
    PlayerListener(wstring ip, INT16 port) : Listener(ip, port, []() { return make_shared<Player>(); })
    {

    }
};

