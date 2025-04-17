#include "../main.h"

CheatManager* pCheatManager = nullptr;
void InjectHooks();

void CheatManager::Initialise()
{
	LOGD("CheatManager::Initialise");

	InjectHooks();

	pCheatManager = new CheatManager();
	pCheatManager->AddCheat<AntiStamina>("AntiStamina");
}

void CheatManager::Uninitialise()
{
	if (pCheatManager) 
	{
		for (auto& cheat : pCheatManager->cheats_) {
			cheat->Disable();
			cheat->OnDisable();
		}

		pCheatManager->cheats_.clear();

		delete pCheatManager;
		pCheatManager = nullptr;
	}
}

void CheatManager::Process() {
	if(pCheatManager)
		pCheatManager->Update();
}

void CheatManager::RenderAll()
{
	if (pCheatManager)
		pCheatManager->Render();
}

CheatManager* CheatManager::GetInstance()
{
	return pCheatManager;
}
