#pragma once
class MatchingManager
{
public:
	
	void AddPlayer(const PlayerRef& pPlayer);
	void RemovePlayer(const PlayerRef& pPlayer);

private:
	vector<PlayerRef> _players;

}; 

extern MatchingManager GPlayerManager;

//실력점수에 따라 정렬, 인접한 유저끼리 매칭 
