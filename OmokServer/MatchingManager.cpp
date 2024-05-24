#include "pch.h"
#include "MatchingManager.h"
#include "Player.h"

MatchingManager GPlayerManager;

void MatchingManager::Match(PlayerRef player)
{
	vector<PlayerRef> list;

	for (auto& p : _players)
	{
		if (p != player && p->waitingForGame && SimilarTo(player->elo, p->elo))
		{
			list.push_back(p);
			list.push_back(player);
			StartGameRoom(list);
			break;
		}
	}
}

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

void MatchingManager::StartGameRoom(const vector<PlayerRef>& players)
{
	cout << "StartGameRoom Calld" << endl;
}

bool MatchingManager::SimilarTo(INT16 leftElo, INT16 rightElo)
{
	return std::abs(leftElo - rightElo) <= 100;
}

double MatchingManager::CalcWinRate(int16_t playerRating, int16_t opponentRating)
{
	return 1.0 / (1.0 + ::pow(10, (opponentRating - playerRating) / 400.0));
}

INT16 MatchingManager::CalcNewRating(int16_t currentRating, double Score, double expectedWinRate, double kFactor)
{
	return INT16();
}

void MatchingManager::UpdateRating(PlayerRef player1, PlayerRef player2, double P1Score)
{
	double p1_expectedWinRate = CalcWinRate(player1->elo, player2->elo);
	double p2_expectedWinRate = CalcWinRate(player2->elo, player1->elo);

	int16_t p1_newRating = CalcNewRating(player1->elo, P1Score, p1_expectedWinRate);
	int16_t p2_newRating= CalcNewRating(player2->elo, 1.0 - P1Score, p2_expectedWinRate);

	player1->elo = p1_newRating;
	player2->elo = p2_newRating;
}
