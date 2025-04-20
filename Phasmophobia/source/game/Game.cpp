#include "../main.h"

void Game::OnMissionStart()
{
	LOGD("Game::OnMissionStart");
	Game::isOnMission = true;
}
extern II::GameObject* gJackalope;
void Game::OnMissionOver()
{
	LOGD("Game::OnMissionOver");
	Game::isOnMission = false;
	LevelController::instance = nullptr;
	gJackalope = nullptr;
	Ghost::Reset();
}

bool Game::IsSinglePlayer()
{
	if (Game::lobbyType == MULTIPLAYER) {
		return false;
	}

	return true;
}
