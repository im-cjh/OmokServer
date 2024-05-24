#pragma once

class MatchingManager
{
public:
    MatchingManager() = default;

public:
    void AddToQueueAndMatch(PlayerRef player);

private:
    void tryMatchmaking();
    void startGameSession(const PlayerRef player1, const PlayerRef player2);
    void communicateWithBattleServer(const PlayerRef player1, const PlayerRef player2);

private:
    queue<PlayerRef> _players;
    mutex queueMutex;
};

extern MatchingManager GMatchMaker;