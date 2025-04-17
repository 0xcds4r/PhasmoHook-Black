#pragma once
#include <memory>
#include <array>
#include <string_view>
#include <mutex>

class NetworkPlayer {
public:
    explicit NetworkPlayer(Player* player, int playerId) {
        player_ = player;
        playerId_ = playerId;
    }

    ~NetworkPlayer() {
        OnDestroy();
    }

    void OnDestroy();

    const char* GetName() const noexcept { return name_.c_str(); }
    bool IsLocal() const noexcept { return isLocal_; }
    Player* GetPlayer() const noexcept { return player_; }

    int GetID() {
        return playerId_;
    }
   
    void SetName(std::string_view name) { name_ = name; }

    void SetLocal(bool bLocal) {
        isLocal_ = true;
    }

    void Draw();

public:
    int playerId_{ -1 };
    Player* player_{ nullptr };
    bool isLocal_{ false };
    std::string name_{ "undefined" };
};

#define MAX_COLORS 1000
class Players
{
private:
    static inline std::mutex s_mutex;
    static inline std::unique_ptr<NetworkPlayer> localPlayer_;
    static inline std::array<std::unique_ptr<NetworkPlayer>, 5> remotePlayers_;

public:
    static void Init();

public:
    static bool IsLocalPlayer(Player* player);
    static std::string GetNickname(Player* player);
    static std::string GetPhotonPlayerNickname(void* owner);

    static bool AddRemotePlayer(Player* player, NetworkPlayer** result) {
        //std::lock_guard lock(s_mutex);
        if (!player) return false;
        
        for (auto& slot : remotePlayers_) {
            if (!slot) {
                slot = std::make_unique<NetworkPlayer>(player, Players::GetActorNumber(player));
                slot->SetName(Players::GetNickname(player));
                slot->SetLocal(false);
                *result = slot.get();
                //Log(std::format("Players::AddRemotePlayer -> Added: {} (ID: {})", std::string(slot->GetName()), std::to_string(slot->GetID())));
                return true;
            }
        }
        return false;
    }

    static std::vector<NetworkPlayer*> GetAllPlayers(bool bIncludeLocal = false) {
        //std::lock_guard lock(s_mutex);
        std::vector<NetworkPlayer*> players;
        players.clear();

        if (localPlayer_ && bIncludeLocal) {
            players.push_back(localPlayer_.get());
        }

        for (const auto& remotePlayer : remotePlayers_) {
            if (remotePlayer) {
                players.push_back(remotePlayer.get());
            }
        }

        return players;
    }

    static bool DeleteRemotePlayer(Player* player) {
        //std::lock_guard lock(s_mutex);
        if (!player) return false;

        for (auto& slot : remotePlayers_) {
            if (slot && slot->GetPlayer() == player) {
                //Log("Players::DeleteRemotePlayer -> Removed: " + std::string(slot->GetName()));
                slot.reset();
                return true;
            }
        }
        return false;
    }

    static NetworkPlayer* GetRemotePlayerAt(size_t index) noexcept {
        //std::lock_guard lock(s_mutex);
        return (index < remotePlayers_.size()) ? remotePlayers_[index].get() : nullptr;
    }

    static int GetActorNumber(Player* player);

    static void SetLocalPlayer(Player* player) {
        //std::lock_guard lock(s_mutex);
        if (player && !localPlayer_) {
            localPlayer_ = std::make_unique<NetworkPlayer>(player, Players::GetActorNumber(player));
            localPlayer_->SetName(Players::GetNickname(player));
            localPlayer_->SetLocal(true);

            //Log(std::format("Players::SetLocalPlayer -> Set: {} (ID: {})", std::string(localPlayer_->GetName()), std::to_string(localPlayer_->GetID())));
            //Log("Players::SetLocalPlayer -> Set: " + std::string(localPlayer_->GetName()));
        }
    }

    static void DestroyLocalPlayer() {
        //std::lock_guard lock(s_mutex);
        if (localPlayer_) {
            //Log("Players::DestroyLocalPlayer -> Removed: " + std::string(localPlayer_->GetName()));
            localPlayer_.reset();
        }
    }

    static NetworkPlayer* GetLocalPlayer() noexcept {
        //std::lock_guard lock(s_mutex);
        return localPlayer_.get();
    }

    static void Render() {
        //std::lock_guard lock(s_mutex);
        if (remotePlayers_.empty()) 
            return;

        for (auto& slot : remotePlayers_) {
            slot->Draw();
        }
    }

private:
    inline static auto UNITY_CALLING_CONVENTION HAwake(Player* _this) -> void;
    inline static auto UNITY_CALLING_CONVENTION HUpdate(Player* _this) -> void;
    inline static auto UNITY_CALLING_CONVENTION HOnDestroy(Player* _this) -> void;
};

