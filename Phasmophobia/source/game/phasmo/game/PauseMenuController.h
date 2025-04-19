#pragma once

class JournalController;
class PauseMenuController : public II::MonoBehaviour
{
public:
	II::GameObject* mainMenu;
	II::GameObject* audioMenu;
	II::GameObject* optionsMenu;
	II::GameObject* playerStats;
	II::GameObject* ghostStats;
	II::GameObject* reportPage;
	II::GameObject* recentPlayers;
	II::GameObject* blockedPlayers;
	II::GameObject* leaveGameButton;
	void* areYouSure;
	JournalController* journalController;
};
VALIDATE_SIZE(PauseMenuController, 0x68 + STRUCT_STUCK);