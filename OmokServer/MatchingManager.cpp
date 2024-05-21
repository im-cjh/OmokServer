#include "pch.h"
#include "MatchingManager.h"

MatchingManager GPlayerManager;

void MatchingManager::AddPlayer(const PlayerRef& pPlayer)
{
	_players.push_back(pPlayer);
}

void MatchingManager::RemovePlayer(const PlayerRef& pPlayer)
{
	auto it = ::find(_players.begin(), _players.end(), pPlayer);
	if (it != _players.end())
		_players.erase(it);
}
