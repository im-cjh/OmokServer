#pragma once

// shared_ptr
using SessionRef = std::shared_ptr<class Session>;
using PacketSessionRef = std::shared_ptr<class PacketSession>;
using ListenerRef = std::shared_ptr<class Listener>;
using PlayerRef = std::shared_ptr<class Player>;
using BattleServerSessionRef = std::shared_ptr<class BattleServerSession>;