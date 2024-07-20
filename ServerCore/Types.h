#pragma once
#define NORMAL_GAME_PLAYER_SIZE 2

// shared_ptr
using SessionRef = std::shared_ptr<class Session>;
using OmokServerSessionRef = std::shared_ptr<class OmokServerSession>;
using BattleServerSessionRef = std::shared_ptr<class BattleServerSession>;
using PacketSessionRef = std::shared_ptr<class PacketSession>;
using ListenerRef = std::shared_ptr<class Listener>;
using BattleServerPlayerRef = std::shared_ptr<class BattleServerPlayer>;
using LobbyPlayerRef = std::shared_ptr<class LobbyPlayer>;
