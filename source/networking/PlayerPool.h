#pragma once

#include "NetworkPlayer.h"
#define MAX_PLAYERS 4

class PlayerPool
{
public:
    NetworkPlayer* players[MAX_PLAYERS]{};

    PlayerPool() {};
    ~PlayerPool() {};
    //inline static Player* localPlayer = nullptr;

    int GetFreeSlot() {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (!players[i]) {
                return i;
            }
        }

        return 255;
    };

    int GetID(Player* player) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (players[i]->GetPlayer() == player) {
                return i;
            }
        }

        return 255;
    }

    NetworkPlayer* GetPlayer(int id) {
        if (id < 0 || id >= MAX_PLAYERS) return nullptr;
        return players[id];
    }

    void RegisterPlayer(Player* player);
    void UnregisterPlayer(Player* player);
};

