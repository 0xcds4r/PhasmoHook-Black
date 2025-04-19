#pragma once

class Ghost
{
public:
	static void Init();
	static void Reset();
	static GhostAI::States GetState(GhostAI* _this);
	static void* GetGhostModel(GhostAI* _this);
	static GhostInfo* GetInfo(GhostAI* _this);
	//static const char* GetTypeNameRus();
	static bool IsHunting();
	static bool IsGhostEvent();
	static bool IncenseEffect();
	static const char* GetBansheeTargetNickname();
	static const char* GetDescription(bool bRussian = false);

	/*template<typename T>
	static inline T GetPointerData(uintptr_t offset) {
		if (!Ghost::gCurrentGhost) return T{};
		return GetOffsetValue<T>((void*)Ghost::gCurrentGhost, offset, "Assembly-CSharp.dll", "GhostAI");
	}

	static std::string GetHexBytes(size_t from, size_t to) {
		if (!Ghost::gCurrentGhost) return "No ghost data";
		if (from > to) return "Invalid range: 'from' must be less than or equal to 'to'";

		std::stringstream ss;
		ss << std::hex << std::setfill('0');  

		for (size_t i = from; i <= to; ++i) {
			unsigned char value = GetPointerData<unsigned char>(i);

			ss << "0x" << std::setw(2) << i << ": "  
				<< std::setw(2) << static_cast<unsigned int>(value);  

			if ((i - from) % 10 == 9 && i < to) {
				ss << "\n";
			}
			else if (i < to) {
				ss << " ";
			}
		}

		return ss.str();
	}*/

	static void SetNewBansheeTarget(Player* player);
	static const char* GetName();
	static GhostTraits::GhostType GetType();
	static GhostTraits::GhostType GetMimicType();
	static const char* GetTypeName(GhostTraits::GhostType type = GhostTraits::GhostType::Spirit, bool bRussianLocale = false);
	static int GetAge();
	static const char* GetSex();
	static int GetFavouriteRoomID();
	static const char* GetStateName();

public:
	static inline std::vector<std::tuple<EMF*, std::chrono::time_point<std::chrono::steady_clock>>> emfData{};
	static inline Player* bansheeTarget = nullptr;
	static inline GhostAI* gCurrentGhost = nullptr;
};

