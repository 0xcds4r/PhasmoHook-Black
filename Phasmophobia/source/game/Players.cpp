#include "../main.h"
#include "../../library/PhotonHelper.h"
#include "../gui/SkeletonESP.h"

SkeletonESP playerESP;

// нуждается в доработке
void Players::Init()
{
    /*try {
        setupHook("Assembly-CSharp.dll", "Player", "Awake", HAwake);
        setupHook("Assembly-CSharp.dll", "Player", "Update", HUpdate);
        setupHook("Assembly-CSharp.dll", "Player", "OnDestroy", HOnDestroy);
    }
    catch (const std::exception& e) {
        Log("Init Exception: " + std::string(e.what()));
    }
    catch (...) {
        Log("Init Unknown Exception");
    }*/
}

bool Players::IsLocalPlayer(Player* player)
{
    if (!player || !player->photonView) return false;

    auto photonView = static_cast<Photon::Pun::PhotonView*>(player->photonView);
    auto localPlayer = (void*)INVOKE_METHOD_EA(uintptr_t, "PhotonUnityNetworking.dll", "PhotonNetwork", "get_LocalPlayer");

    return photonView->GetOwner() == localPlayer;
}

std::string Players::GetPhotonPlayerNickname(void* owner) {
    auto ownerWay = (Photon::Realtime::Player*)owner;
    if (ownerWay && ownerWay->GetNickName()) {
        return std::format("{}", ownerWay->GetNickName()->ToString());
    }
    return "undefined";
}

std::string Players::GetNickname(Player* player) {
    auto photonView = (Photon::Pun::PhotonView*)player->photonView;
    if (photonView) {
        auto owner = (Photon::Realtime::Player*)photonView->Owner;
        if (owner && owner->GetNickName()) {
            return std::format("{}", owner->GetNickName()->ToString());
        }
        return "undefined";
    }
    return "undefined";
}

int Players::GetActorNumber(Player* player) {
    auto photonView = (Photon::Pun::PhotonView*)player->photonView;
    if (photonView) {
        auto owner = photonView->GetOwner();
        if (owner) {
            return owner->GetActorNumber();
        }
    }

    return -1;
}

inline auto UNITY_CALLING_CONVENTION Players::HAwake(Player* _this) -> void
{
    try {
        H::Fcall(HAwake, _this);
        if (!Game::bPhotonAwaken || !_this) return;

        if (IsLocalPlayer(_this)) {
            if (!GetLocalPlayer()) {
                SetLocalPlayer(_this);
            }
        }
        else {
            NetworkPlayer* result = nullptr;
            AddRemotePlayer(_this, &result);
        }
    }
    catch (const std::exception& e) {
        //Log("Players::HAwake Exception: " + std::string(e.what()));
    }
}

extern bool noclip_initialized;
void DoNoClip(Player* player) 
{ // BAZA
    if (!player) {
        return;
    }

    auto gameObject = player->GetGameObject();
    if (!gameObject) {
        return;
    }

    auto transform = player->GetTransform();
    if (!transform) {
        return;
    }

    auto camera = II::Camera::GetMain();
    if (!camera || !camera->GetTransform()) {
        return;
    }

    auto cameraTransform = camera->GetTransform();

    static UnityResolve::UnityType::Vector3 staticPosition = transform->GetPosition();

    if (!noclip_initialized) {
        staticPosition = transform->GetPosition();
        noclip_initialized = true;
    }

    float baseSpeed = 5.0f;
    float speedMultiplier = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) ? 2.0f : 1.0f; 
    float speed = baseSpeed * speedMultiplier;
    float deltaTime = UnityResolve::UnityType::Time::GetDeltaTime();

    UnityResolve::UnityType::Vector3 movement{ 0.0f, 0.0f, 0.0f };
    UnityResolve::UnityType::Vector3 forward = cameraTransform->GetForward();
    UnityResolve::UnityType::Vector3 right = cameraTransform->GetRight();
    UnityResolve::UnityType::Vector3 up = { 0.0f, 1.0f, 0.0f }; 

    UnityResolve::UnityType::Vector3 flatForward = forward;
    flatForward.y = 0.0f;
    flatForward = flatForward.Normalize();

    UnityResolve::UnityType::Vector3 flatRight = right;
    flatRight.y = 0.0f;
    flatRight = flatRight.Normalize();

    bool useCameraDirection = true;

    if (!useCameraDirection) {
        if (GetAsyncKeyState('W') & 0x8000) { // Вперёд
            movement = movement + (flatForward * speed * deltaTime);
        }
        else if (GetAsyncKeyState('S') & 0x8000) { // Назад
            movement = movement - (flatForward * speed * deltaTime);
        }
        else if (GetAsyncKeyState('A') & 0x8000) { // Влево
            movement = movement - (flatRight * speed * deltaTime);
        }
        else if (GetAsyncKeyState('D') & 0x8000) { // Вправо
            movement = movement + (flatRight * speed * deltaTime);
        }

        if (GetAsyncKeyState(VK_SPACE) & 0x8000) { // Вверх
            movement = movement + (up * speed * deltaTime);
        }
        else if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) { // Вниз
            movement = movement - (up * speed * deltaTime);
        }
    }
   
    if (useCameraDirection) {
        if (GetAsyncKeyState('W') & 0x8000) {
            movement = movement + (forward * speed * deltaTime);
        }
        else if (GetAsyncKeyState('S') & 0x8000) {
            movement = movement - (forward * speed * deltaTime);
        }
    }

    staticPosition = staticPosition + movement;
    transform->SetPosition(staticPosition);
}

inline auto UNITY_CALLING_CONVENTION Players::HUpdate(Player* _this) -> void
{
    try {
        if (Game::isOnMission && 
            Ghost::gCurrentGhost && 
            ApplicationInfo::bCheatEnabled[CHEAT_NOCLIP]
            && Players::GetLocalPlayer() && Players::GetLocalPlayer()->GetPlayer() == _this) 
        {
            DoNoClip(_this);
            return;
        }

        H::Fcall(HUpdate, _this);
    }
    catch (const std::exception& e) {
        //Log("Players::HUpdate Exception: " + std::string(e.what()));
    }
}

inline auto UNITY_CALLING_CONVENTION Players::HOnDestroy(Player* _this) -> void
{
    try {
        if (_this) {
            if (auto* local = GetLocalPlayer(); local && local->GetPlayer() == _this) {
                DestroyLocalPlayer();
            }
            else {
                DeleteRemotePlayer(_this);
            }
        }
        H::Fcall(HOnDestroy, _this);
    }
    catch (const std::exception& e) {
        //Log("Players::HOnDestroy Exception: " + std::string(e.what()));
    }
}

ImU32 GenerateColor(int index) {
    float hue = static_cast<float>(index * 137 % 360); 
    float saturation = 0.8f; 
    float value = 1.0f;     

    int h = static_cast<int>(hue / 60.0f) % 6;
    float f = hue / 60.0f - h;
    float p = value * (1.0f - saturation);
    float q = value * (1.0f - f * saturation);
    float t = value * (1.0f - (1.0f - f) * saturation);

    int r, g, b;
    switch (h) {
    case 0: r = value * 255, g = t * 255, b = p * 255; break;
    case 1: r = q * 255, g = value * 255, b = p * 255; break;
    case 2: r = p * 255, g = value * 255, b = t * 255; break;
    case 3: r = p * 255, g = q * 255, b = value * 255; break;
    case 4: r = t * 255, g = p * 255, b = value * 255; break;
    case 5: r = value * 255, g = p * 255, b = q * 255; break;
    default: r = 255, g = 255, b = 255; break;
    }

    return IM_COL32(r, g, b, 255);
}

void NetworkPlayer::Draw() {

    auto camera = II::Camera::GetMain();
    if (!camera) {
        LOGD("NetworkPlayer::Draw -> No main camera available");
        return;
    }

    auto gamePlayer = this->GetPlayer();
    if (!gamePlayer) {
        LOGD("NetworkPlayer::Draw -> Player is null");
        return;
    }
    if (!this->GetName()) {
        LOGD("NetworkPlayer::Draw -> Player name is null");
        return;
    }

    auto gameObject = gamePlayer->GetGameObject();
    if (!gameObject) {
        //Log("NetworkPlayer::Draw -> GameObject is null for player: " + std::string(this->GetName()));
        return;
    }

    auto transform = gamePlayer->GetTransform();
    if (!transform) {
        //Log("NetworkPlayer::Draw -> Transform is null for player: " + std::string(this->GetName()));
        return;
    }

    II::Vector3 position = transform->GetPosition();
    II::Vector3 up = transform->GetUp();

    if (position.isZero()) {
        //Log("NetworkPlayer::Draw -> Position is zero for player: " + std::string(this->GetName()));
        return;
    }

    float heightOffset = 0.97f;
    std::string nameTag = std::format("({}) {}", this->GetID(), this->GetName());

    II::Vector3 headPosition;
    bool headFound = false;

    II::Animator* pAnimatorSaved = nullptr;

    FOR_EACH_COMPONENT(gamePlayer, II::Animator, "UnityEngine.AnimationModule.dll", "Animator", pAnimator) {
        if (pAnimator) {
            if (!pAnimator->GetEnabled()) {
                //Log("NetworkPlayer::Draw -> Animator found but disabled for player: " + nameTag);
                continue;
            }
            auto headTransform = pAnimator->GetBoneTransform(II::Animator::HumanBodyBones::Head);
            if (headTransform) {
                headPosition = headTransform->GetPosition();
                headPosition.y += 0.25f; // optimal range
                pAnimatorSaved = pAnimator;
                headFound = true;
                break;
            }
            else {
                //Log("NetworkPlayer::Draw -> No Head bone found in Animator for player: " + nameTag);
            }
        }
        else {
            //Log("NetworkPlayer::Draw -> Animator component is null for player: " + nameTag);
        }
    };

    if (!headFound) {
        headPosition = position + (up * heightOffset);
    }

    auto screenPos = camera->WorldToScreenPoint(headPosition, UnityResolve::UnityType::Camera::Eye::Mono);

    if (screenPos.z <= 0) {
        //Log("NetworkPlayer::Draw -> Head position is behind camera for player: " + nameTag +
            //" (screenPos.z = " + std::to_string(screenPos.z) + ")");
        return;
    }

    screenPos.y = ApplicationInfo::screenHeight - screenPos.y;
    ImVec2 textSize = ImGui::CalcTextSize(nameTag.c_str());

    ImVec2 textPos = ImVec2(
        screenPos.x - textSize.x * 0.5f,
        screenPos.y - textSize.y - 5.0f
    );

    auto drawOutlinedText = [&](const std::string& tag, const ImVec2& pos, ImU32 textColor, ImU32 outlineColor) {
        auto* drawList = ImGui::GetBackgroundDrawList();
        float outlineOffset = 1.0f; 

        drawList->AddText(ImVec2(pos.x - outlineOffset, pos.y - outlineOffset), outlineColor, nameTag.c_str());
        drawList->AddText(ImVec2(pos.x, pos.y - outlineOffset), outlineColor, nameTag.c_str());
        drawList->AddText(ImVec2(pos.x + outlineOffset, pos.y - outlineOffset), outlineColor, nameTag.c_str());
        drawList->AddText(ImVec2(pos.x - outlineOffset, pos.y), outlineColor, nameTag.c_str());
        drawList->AddText(ImVec2(pos.x + outlineOffset, pos.y), outlineColor, nameTag.c_str());
        drawList->AddText(ImVec2(pos.x - outlineOffset, pos.y + outlineOffset), outlineColor, nameTag.c_str());
        drawList->AddText(ImVec2(pos.x, pos.y + outlineOffset), outlineColor, nameTag.c_str());
        drawList->AddText(ImVec2(pos.x + outlineOffset, pos.y + outlineOffset), outlineColor, nameTag.c_str());

        drawList->AddText(pos, textColor, tag.c_str());
    };

    ImU32 textColor = GenerateColor(GetID());
    ImU32 outlineColor = IM_COL32(0, 0, 0, 255);  
    drawOutlinedText(nameTag, textPos, textColor, outlineColor);

    /*if (pAnimatorSaved) {
        playerESP.DrawBoneConnectionsNoCache(this->GetID(), pAnimatorSaved,
            (textColor >> 16) & 0xFF, 
            (textColor >> 8) & 0xFF,  
            textColor & 0xFF,         
            255);                      
    }*/
}

void NetworkPlayer::OnDestroy() {
    //playerESP.ClearAllBoneCache();
}

