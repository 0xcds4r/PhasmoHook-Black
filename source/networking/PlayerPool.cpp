#include "../main.h"

// we need a set by actor id

void PlayerPool::RegisterPlayer(Player* player)
{
    try {
        int i = GetFreeSlot();
        if (i == 255) {
            return;
        }

        if (players[i]) { //wtflol
            return;
        }

        Log(std::format("PlayerPool RegisterPlayer: {}", i));
        players[i] = new NetworkPlayer(player); 
        if (players[i])
        {
            const char* username = Players::GetUsername(player);
            if (username)
            {
                players[i]->SetName(username);
                players[i]->setLocal(Players::IsLocalPlayer(player));
            }
        }
    }
    catch (const std::exception& e) {
        Log(std::format("Exception in RegisterPlayer: {}", e.what()));
    }
    catch (...) {
        Log("Unknown exception in RegisterPlayer.");
    }
}

void PlayerPool::UnregisterPlayer(Player* player)
{
    try {
        int id = GetID(player);
        if (id >= MAX_PLAYERS) {
            return;
        }

        if (players[id] && players[id]->GetPlayer() && players[id]->GetName())
        {
            if (players[id]->GetPlayer() == player) {
                Log(std::format("PlayerPool Unregister Player: {}", id));
                delete players[id]; 
                players[id] = nullptr;
            }
        }
    }
    catch (const std::exception& e) {
        Log(std::format("Exception in UnregisterPlayer: {}", e.what()));
    }
    catch (...) {
        // ? ? ?
        Log("Unknown exception in UnregisterPlayer.");
    }
}