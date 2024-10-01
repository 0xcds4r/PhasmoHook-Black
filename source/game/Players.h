#pragma once

class Players
{
public:
    static void Init();
    static void Reset();

public:
    /*inline static std::vector<Player*> gPlayersList{};
    inline static std::mutex playersMutex{};

    static Player* GetPlayerById(int playerId)
    {
        std::lock_guard lock(playersMutex);
        if (gPlayersList.empty()) {
            return nullptr;
        }

        for (Player* player : gPlayersList) {
            if (player && GetPlayerId(player) == playerId) {
                return player;
            }
        }
        return nullptr;
    }

    static int GetPlayerId(Player* p)
    {
        std::lock_guard lock(playersMutex);
        if (gPlayersList.empty()) {
            return 255;
        }

        int id = 0;
        for (Player* player : gPlayersList) {
            if (p == player) {
                return id;
            }
            id++;
        }

        return 255;
    }

    static int GetOnlinePlayersCount() {
        std::lock_guard lock(playersMutex);
        if (gPlayersList.empty()) {
            return 0;
        }

        int i = 0;
        for (Player* player : gPlayersList) {
            if (player) i++;
        }

        return i;
    }

    static bool IsValidPlayer(Player* player)
    {
        return player && GetUsername(player) && strlen(GetUsername(player)) > 0;
    }*/

    static bool IsLocalPlayer(Player* player);

    static Player* GetLocalPlayer();
    static const char* GetUsername(Player* player);

    static bool IsValidData(Player* player);

    static const char* GetCurrentRoomName(Player* player);

    static inline Player* gLocalPlayer = nullptr;

private:
    inline static auto UNITY_CALLING_CONVENTION  HAwake(Player* _this) -> void;
    inline static auto UNITY_CALLING_CONVENTION  HUpdate(Player* _this) -> void;
    inline static auto UNITY_CALLING_CONVENTION  HOnDestroy(Player* _this) -> void;
};