#include "../main.h"

extern LevelController* gLevelController;
extern DNAEvidence* pBone;

void Game::OnMissionStart()
{
	//Log("Game::OnMissionStart");
	Game::isOnMission = true;
}

void Game::OnMissionOver()
{
	//Log("Game::OnMissionOver");
	Game::isOnMission = false;
	
	gLevelController = nullptr;
	pBone = nullptr;

	Ghost::Reset();
}

bool Game::IsSinglePlayer()
{
	if (Game::lobbyType == MULTIPLAYER) {
		return false;
	}

	return true;
}
