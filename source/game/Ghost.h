#pragma once

const std::map<GhostType, const char*> GhostTypeNames = {
	{GhostType::Spirit, "Spirit"},
	{GhostType::Wraith, "Wraith"},
	{GhostType::Phantom, "Phantom"},
	{GhostType::Poltergeist, "Poltergeist"},
	{GhostType::Banshee, "Banshee"},
	{GhostType::Jinn, "Jinn"},
	{GhostType::Mare, "Mare"},
	{GhostType::Revenant, "Revenant"},
	{GhostType::Shade, "Shade"},
	{GhostType::Demon, "Demon"},
	{GhostType::Yurei, "Yurei"},
	{GhostType::Oni, "Oni"},
	{GhostType::Yokai, "Yokai"},
	{GhostType::Hantu, "Hantu"},
	{GhostType::Goryo, "Goryo"},
	{GhostType::Myling, "Myling"},
	{GhostType::Onryo, "Onryo"},
	{GhostType::TheTwins, "TheTwins"},
	{GhostType::Raiju, "Raiju"},
	{GhostType::Obake, "Obake"},
	{GhostType::Mimic, "Mimic"},
	{GhostType::Moroi, "Moroi"},
	{GhostType::Deogen, "Deogen"},
	{GhostType::Thaye, "Thaye"}
};

const std::map<GhostState, const char*> GhostStateNames = {
	{GhostState::Idle, "Idle"},
	{GhostState::Wander, "Wander"},
	{GhostState::Hunting, "Hunting"},
	{GhostState::FavouriteRoom, "Favourite Room"},
	{GhostState::Light, "Light"},
	{GhostState::Door, "Door"},
	{GhostState::Throwing, "Throwing"},
	{GhostState::Fusebox, "Fusebox"},
	{GhostState::Appear, "Appear"},
	{GhostState::DoorKnock, "Door Knock"},
	{GhostState::WindowKnock, "Window Knock"},
	{GhostState::CarAlarm, "Car Alarm"},
	{GhostState::Flicker, "Flicker"},
	{GhostState::Cctv, "CCTV"},
	{GhostState::RandomEvent, "Random Event"},
	{GhostState::GhostAbility, "Ghost Ability"},
	{GhostState::Mannequin, "Mannequin"},
	{GhostState::TeleportObject, "Teleport Object"},
	{GhostState::Interact, "Interact"},
	{GhostState::SummoningCircle, "Summoning Circle"},
	{GhostState::MusicBox, "Music Box"},
	{GhostState::Dots, "Dots"},
	{GhostState::Salt, "Salt"}
};

class Ghost
{
public:
	static void Init();
	static void Reset();
	static GhostState GetState(GhostAI* _this);
	static void* GetGhostModel(GhostAI* _this);
	static GhostInfo* GetInfo(GhostAI* _this);

	static bool GetHunting(GhostAI* _this);
	static bool IsHunting();
	
	static const char* GetName();
	static const char* GetTypeName();
	static int GetAge();
	static const char* GetSex();
	static int GetFavouriteRoomID();
	static const char* GetStateName();

	static inline std::vector<std::tuple<EMF*, std::chrono::time_point<std::chrono::steady_clock>>> emfData{};
public:
	static inline GhostAI* gCurrentGhost = nullptr;
private:
	inline static auto UNITY_CALLING_CONVENTION  HAwake(GhostAI* _this) -> void;
	inline static auto UNITY_CALLING_CONVENTION  HUpdate(GhostAI* _this) -> void;
};

