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

//�Ƿ������� ���� ����, ������ �������� ��Ī 
