#pragma once

#define STRUCT_STUCK 0x10

#define PAD(size) \
    char pad_##__LINE__[static_cast<std::size_t>(size)];

#define VALIDATE_SIZE(clazz, expected_size) \
    static_assert(sizeof(clazz) == expected_size, "size of " #clazz " must be equal " #expected_size " bytes")

#include "phasmo/phasmo_enums.h"
#include "phasmo/phasmo_interfaces.h"
#include "phasmo/phasmo_structs.h"

#include "Ghost.h"
#include "Room.h"
#include "Players.h"

enum eLobbyType {
	MAIN,
	SINGLEPLAYER,
	MULTIPLAYER
};

class Game {
public:
	static inline bool bPhotonAwaken = false;
	static inline eLobbyType lobbyType = eLobbyType::MAIN;
	static inline bool isOnMission = false;
public:
	static void OnMissionStart();
	static void OnMissionOver();
	static bool IsSinglePlayer();
};