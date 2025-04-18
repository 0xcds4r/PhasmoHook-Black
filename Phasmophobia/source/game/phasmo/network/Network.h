#pragma once

enum PlatformType {
    steam,
    viveport,
    PlayStation,
    Xbox
};

class Network : II::MonoBehaviourPun {
public:
    struct PlayerSpot {
        bool playerReady; // Offset: 0x10
        int playerCharacterIndex; // Offset: 0x14
        void* photonPlayer; // Offset: 0x18
        II::String* unityPlayerID; // Offset: 0x20
        int experience; // Offset: 0x28
        int level; // Offset: 0x2C
        int prestige; // Offset: 0x30
        Player* player; // Offset: 0x38
        float playerVolume; // Offset: 0x40
        II::String* accountName; // Offset: 0x48
        bool isKicked; // Offset: 0x50
        bool isHacker; // Offset: 0x51
        bool isBlocked; // Offset: 0x52
        void* roleBadges; // Offset: 0x58
        void* role; // Offset: 0x60
        int prestigeIndex; // Offset: 0x64
        bool prestigeTheme; // Offset: 0x68
        void* votedContract; // Offset: 0x70
        PlatformType platformType; // Offset: 0x78
        bool hasReceivedPlayerInformation; // Offset: 0x7C
        bool playerIsBlocked; // Offset: 0x7D
        int legacyLevel; // Offset: 0x80
        int legacyAccent; // Offset: 0x84
        int legacyBackground; // Offset: 0x88
        int legacyColour; // Offset: 0x8C
        void* playerEquipment; // Offset: 0x90
        bool hasBroughtItems; // Offset: 0x98
        int totalEquipmentCost; // Offset: 0x9C
        void* onBlockMuteStateChanged; // Offset: 0xA0
    };
    VALIDATE_SIZE(PlayerSpot, 0x98);

    // ----------------------------------------------------------
    static inline Network* instance = nullptr;
    static Network* GetInstance() {
        return instance;
    }

    // ----------------------------------------------------------
    Player* localPlayer; // Offset: 0x28
    II::List<PlayerSpot*>* playersData; // Offset: 0x30
    void* playerColors; // Offset: 0x38
    void* defaultPlayerColors; // Offset: 0x40
    void* JoinedLobby; // Offset: 0x48
    void* JoinedRoom; // Offset: 0x50
    void* LeftRoom; // Offset: 0x58
    void* Disconnected; // Offset: 0x60
    void* OnMicrophoneChanged; // Offset: 0x68
    void* OnMasterClientSwitchedEvent; // Offset: 0x70
    void* OnPlayerEnteredRoomEvent; // Offset: 0x78
    void* OnPlayerLeftRoomEvent; // Offset: 0x80
    void* OnPlayerSpotAssigned; // Offset: 0x88
    void* OnLocalPlayerSpawned; // Offset: 0x90
    void* OnSeasonalEventCompleted; // Offset: 0x98
    void* OnPlayerColorChanged; // Offset: 0xA0, 
    //const uint8_t eventCode = 1; // Offset: 0xA4 
    void* m_levelSystem; // Offset: 0xA8
    bool isChangingScenes; // Offset: 0xB0
    II::String* lastOnlineRoomName; // Offset: 0xB8
    void* recentKickedFromRooms; // Offset: 0xC0
    bool failedToConnectToVivox; // Offset: 0xC8
    bool isAttemptingVivoxLogin; // Offset: 0xC9
    void* inviteSystem; // Offset: 0xD0
    bool isChangingRegion; // Offset: 0xD8
    II::String* changeToRegionOnDisconnect; // Offset: 0xE0 

    auto GetPlayerSpots() -> std::vector<PlayerSpot*> {
        return Network::GetInstance()->playersData->ToArray()->ToVector();
    }

    auto GetLocalPlayer() -> Player* {
        return localPlayer;
    }

    auto GetPlayerIndex(Player* player) -> int 
    {
        if (!player) return -1;
        auto playerSpots = GetPlayerSpots();
        if (playerSpots.empty()) return -1;

        for (int i = 0; i < playerSpots.size(); i++) 
        {
            if (!playerSpots[i]) continue;
            if (!playerSpots[i]->player) continue;

            if (playerSpots[i]->player == player) 
            {
                return i;
            }
        }

        return -1;
    }

    auto GetPlayerSpot(Player* player) -> PlayerSpot*
    {
        auto playerSpots = GetPlayerSpots();
        if (playerSpots.empty()) 
            return nullptr;

        return playerSpots[GetPlayerIndex(player)];
    }

    // i need second account for normally test itt
    auto GetPlayerName(Player* player) -> const char*
    {
        PlayerSpot* spot = GetPlayerSpot(player);
        if (spot == nullptr) {
            return "";
        }

        II::String* pAccountNameString = spot->accountName;
        if (!pAccountNameString) {
            return "";
        }
        std::string strNick = pAccountNameString->ToString();
        return strNick.c_str();
    }
};
VALIDATE_SIZE(Network, 0xD8 + STRUCT_STUCK);