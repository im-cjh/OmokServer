#pragma once
class MatchingManager
{
public:
	void Match(PlayerRef player);
	void AddPlayer(const PlayerRef& pPlayer);
	void RemovePlayer(const PlayerRef& pPlayer);
	void StartGameRoom(const vector<PlayerRef>& players);

	//elo����
public:
	bool SimilarTo(INT16 leftElo, INT16 rightElo);
	double CalcWinRate(int16_t playerRating, int16_t opponentRating);
	INT16 CalcNewRating(int16_t currentRating, double Score, double expectedWinRate, double kFactor = 32);
	void UpdateRating(PlayerRef player1, PlayerRef player2, double P1Score);

private:
	vector<PlayerRef> _players;

}; 

extern MatchingManager GPlayerManager;

//�Ƿ������� ���� ����, ������ �������� ��Ī 
