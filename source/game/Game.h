#pragma once

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
	static inline eLobbyType lobbyType = eLobbyType::MAIN;
	static inline bool isOnMission = false;
public:
	static void OnMissionStart();
	static void OnMissionOver();
	static bool IsSinglePlayer();
};