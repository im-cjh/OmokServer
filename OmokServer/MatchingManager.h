#pragma once

class MatchingManager
{
public:

	MatchingManager() = default;

public:
	void AddToQueueAndMatch(LobbyPlayerRef player);

private:
	void tryMatchmaking();
	void communicateWithBattleServer(const LobbyPlayerRef player1, const LobbyPlayerRef player2);

private:
	int roomID = 1000;
	queue<LobbyPlayerRef> _players;
	mutex queueMutex;
};

extern MatchingManager GMatchMaker;
