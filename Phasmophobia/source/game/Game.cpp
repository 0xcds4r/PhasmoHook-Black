#include "../main.h"
#include "../utils/Event.h"

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
	
	Events::ResetInstances();
	Ghost::ResetData();
	
	// TODO: delete after EE end
	gJackalope = nullptr;
}

bool Game::IsSinglePlayer()
{
	if (Game::lobbyType == MULTIPLAYER) {
		return false;
	}

	return true;
}
