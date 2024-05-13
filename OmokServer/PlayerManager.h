#pragma once
class MatchingManager
{
public:
	

private:
	unordered_map<int, PlayerRef> _players;
}; 

extern MatchingManager GPlayerManager;

//실력점수에 따라 정렬, 인접한 유저끼리 매칭 
