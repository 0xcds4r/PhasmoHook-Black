#pragma once

#include <map>
#include <stdexcept>
#include <vector>

struct BonePair {
    UnityResolve::UnityType::Animator::HumanBodyBones start;
    UnityResolve::UnityType::Animator::HumanBodyBones end;

    bool operator<(const BonePair& other) const {
        if (start != other.start) {
            return start < other.start;
        }
        return end < other.end;
    }
};

constexpr std::array<BonePair, 20> bonePairs = { {
    {II::Animator::HumanBodyBones::Head, II::Animator::HumanBodyBones::Neck},
    {II::Animator::HumanBodyBones::Neck, II::Animator::HumanBodyBones::UpperChest},
    {II::Animator::HumanBodyBones::UpperChest, II::Animator::HumanBodyBones::Chest},
    {II::Animator::HumanBodyBones::Chest, II::Animator::HumanBodyBones::Spine},
    {II::Animator::HumanBodyBones::Spine, II::Animator::HumanBodyBones::Hips},
    {II::Animator::HumanBodyBones::LeftShoulder, II::Animator::HumanBodyBones::LeftUpperArm},
    {II::Animator::HumanBodyBones::LeftUpperArm, II::Animator::HumanBodyBones::LeftLowerArm},
    {II::Animator::HumanBodyBones::LeftLowerArm, II::Animator::HumanBodyBones::LeftHand},
    {II::Animator::HumanBodyBones::RightShoulder, II::Animator::HumanBodyBones::RightUpperArm},
    {II::Animator::HumanBodyBones::RightUpperArm, II::Animator::HumanBodyBones::RightLowerArm},
    {II::Animator::HumanBodyBones::RightLowerArm, II::Animator::HumanBodyBones::RightHand},
    {II::Animator::HumanBodyBones::LeftUpperLeg, II::Animator::HumanBodyBones::LeftLowerLeg},
    {II::Animator::HumanBodyBones::LeftLowerLeg, II::Animator::HumanBodyBones::LeftFoot},
    {II::Animator::HumanBodyBones::LeftFoot, II::Animator::HumanBodyBones::LeftToes},
    {II::Animator::HumanBodyBones::RightUpperLeg, II::Animator::HumanBodyBones::RightLowerLeg},
    {II::Animator::HumanBodyBones::RightLowerLeg, II::Animator::HumanBodyBones::RightFoot},
    {II::Animator::HumanBodyBones::RightFoot, II::Animator::HumanBodyBones::RightToes},
    {II::Animator::HumanBodyBones::LeftEye, II::Animator::HumanBodyBones::Head},
    {II::Animator::HumanBodyBones::RightEye, II::Animator::HumanBodyBones::Head},
    {II::Animator::HumanBodyBones::Jaw, II::Animator::HumanBodyBones::Head}
} };

class SkeletonESP {
private:
    struct EntityData {
        float lastUpdateTime = 0.0f;
        float updateInterval = 0.05f; 
        float currentFPS = 0.0f;
        std::map<BonePair, std::pair<ImVec2, ImVec2>> bonePositions; 
    };

    float defaultUpdateInterval;
    std::map<int, EntityData> entityCache;

    void UpdateFPS(EntityData& entity) {
        float deltaTime = UnityResolve::UnityType::Time::GetDeltaTime();
        if (deltaTime > 0.0f) { 
            entity.currentFPS = 1.0f / deltaTime;
            entity.updateInterval = 1.0f / entity.currentFPS;
        }
        else {
            entity.currentFPS = 0.0f;
            entity.updateInterval = defaultUpdateInterval; 
        }
    }

    void UpdateBonePositions(int entityId, II::Animator* pAnimator, II::Camera* camera) {
        if (!pAnimator || !camera || !pAnimator->GetEnabled()) {
            entityCache.erase(entityId);
            return;
        }

        auto cameraPos = camera->GetTransform()->GetPosition();
        if (!camera->GetTransform()) {
            entityCache.erase(entityId);
            return;
        }

        auto& entity = entityCache[entityId]; 
        auto& entityBoneCache = entity.bonePositions;

        for (const auto& pair : bonePairs) {
            auto* startTransform = pAnimator->GetBoneTransform(pair.start);
            auto* endTransform = pAnimator->GetBoneTransform(pair.end);

            if (!startTransform || !endTransform) {
                entityBoneCache.erase(pair); 
                continue;
            }

            auto startPos = startTransform->GetPosition();
            auto endPos = endTransform->GetPosition();

            auto startPoint = camera->WorldToScreenPoint(startPos, UnityResolve::UnityType::Camera::Eye::Mono);
            auto endPoint = camera->WorldToScreenPoint(endPos, UnityResolve::UnityType::Camera::Eye::Mono);

            if (startPoint.z > 0 && endPoint.z > 0 &&
                std::isfinite(startPoint.x) && std::isfinite(startPoint.y) &&
                std::isfinite(endPoint.x) && std::isfinite(endPoint.y)) {

                startPoint.y = ApplicationInfo::screenHeight - startPoint.y;
                endPoint.y = ApplicationInfo::screenHeight - endPoint.y;

                if (startPoint.x >= 0 && startPoint.x <= ApplicationInfo::screenWidth &&
                    startPoint.y >= 0 && startPoint.y <= ApplicationInfo::screenHeight &&
                    endPoint.x >= 0 && endPoint.x <= ApplicationInfo::screenWidth &&
                    endPoint.y >= 0 && endPoint.y <= ApplicationInfo::screenHeight) {

                    entityBoneCache[pair] = { ImVec2(startPoint.x, startPoint.y), ImVec2(endPoint.x, endPoint.y) };
                }
                else {
                    entityBoneCache.erase(pair); 
                }
            }
            else {
                entityBoneCache.erase(pair); 
            }
        }

        if (entityBoneCache.empty()) {
            entityCache.erase(entityId);
        }
    }

public:
    explicit SkeletonESP(float initialUpdateInterval = 0.05f)
        : defaultUpdateInterval(initialUpdateInterval) {
    }

    void DrawBoneConnections(int entityId, II::Animator* pAnimator, int r = 255, int g = 255, int b = 255, int a = 255) {
        float currentTime = UnityResolve::UnityType::Time::GetTime();
        auto& entity = entityCache[entityId]; 

        if (currentTime - entity.lastUpdateTime >= entity.updateInterval) {
            UpdateFPS(entity);
            if (pAnimator && II::Camera::GetMain()) {
                UpdateBonePositions(entityId, pAnimator, II::Camera::GetMain());
            }
            entity.lastUpdateTime = currentTime;
        }

        if (!pAnimator || !pAnimator->GetEnabled() || !II::Camera::GetMain()) return;

        auto* drawList = ImGui::GetBackgroundDrawList();
        ImU32 color = IM_COL32(r, g, b, a);

        try {
            auto it = entityCache.find(entityId);
            if (it != entityCache.end()) {
                for (const auto& [pair, points] : it->second.bonePositions) {
                    drawList->AddLine(points.first, points.second, color, 1.5f);
                }
            }
        }
        catch (const std::exception& e) {
            printf("Error in DrawBoneConnections: %s\n", e.what());
        }
    }

    bool IsValidScreenPosition(const II::Vector3& point, int screenWidth, int screenHeight) {
        if (!std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z)) {
            return false;
        }

        if (point.z <= 0) {
            return false;
        }

        float screenY = static_cast<float>(screenHeight) - point.y;
        return (point.x >= 0 && point.x <= static_cast<float>(screenWidth) &&
            screenY >= 0 && screenY <= static_cast<float>(screenHeight));
    }

    void DrawBoneConnectionsNoCache(int entityId, II::Animator* pAnimator, int r = 255, int g = 255, int b = 255, int a = 255) {
        if (!pAnimator) {
            return;
        }

        II::Camera* camera = II::Camera::GetMain();
        if (!camera || !camera->GetTransform()) {
            return;
        }

        ImGuiIO& io = ImGui::GetIO();
        if (!io.DisplaySize.x || !io.DisplaySize.y) {
            return;
        }

        auto* drawList = ImGui::GetBackgroundDrawList();
        if (!drawList) {
            return;
        }

        ImU32 color = IM_COL32(r, g, b, a);
        const int screenWidth = static_cast<int>(io.DisplaySize.x);
        const int screenHeight = static_cast<int>(io.DisplaySize.y);

        try {
            for (const auto& pair : bonePairs) {
                auto* startTransform = pAnimator->GetBoneTransform(pair.start);
                auto* endTransform = pAnimator->GetBoneTransform(pair.end);
                if (!startTransform || !endTransform) {
                    continue;
                }

                auto startPos = startTransform->GetPosition();
                auto endPos = endTransform->GetPosition();

                if (!std::isfinite(startPos.x) || !std::isfinite(startPos.y) || !std::isfinite(startPos.z) ||
                    !std::isfinite(endPos.x) || !std::isfinite(endPos.y) || !std::isfinite(endPos.z)) {
                    continue;
                }

                auto startPoint = camera->WorldToScreenPoint(startPos, UnityResolve::UnityType::Camera::Eye::Mono);
                auto endPoint = camera->WorldToScreenPoint(endPos, UnityResolve::UnityType::Camera::Eye::Mono);

                if (IsValidScreenPosition(startPoint, screenWidth, screenHeight) &&
                    IsValidScreenPosition(endPoint, screenWidth, screenHeight)) {

                    drawList->AddLine(
                        ImVec2(startPoint.x, screenHeight - startPoint.y),
                        ImVec2(endPoint.x, screenHeight - endPoint.y),
                        color,
                        1.5f
                    );
                }
            }
        }
        catch (const std::exception& e) {
            printf("Error in DrawBoneConnectionsNoCache: %s\n", e.what());
        }
    }

    void DrawMultipleBoneConnections(const std::vector<std::pair<int, II::Animator*>>& entities,
        int r = 255, int g = 255, int b = 255, int a = 255) {
        float currentTime = UnityResolve::UnityType::Time::GetTime();
        II::Camera* camera = II::Camera::GetMain();
        if (!camera) return;

        auto* drawList = ImGui::GetBackgroundDrawList();
        if (!drawList) return;

        ImU32 color = IM_COL32(r, g, b, a);

        try {
            for (const auto& [entityId, pAnimator] : entities) {
                if (!pAnimator || !pAnimator->GetEnabled()) continue;

                auto& entity = entityCache[entityId]; 

                if (currentTime - entity.lastUpdateTime >= entity.updateInterval) {
                    UpdateFPS(entity);
                    UpdateBonePositions(entityId, pAnimator, camera);
                    entity.lastUpdateTime = currentTime;
                }

                auto it = entityCache.find(entityId);
                if (it != entityCache.end()) {
                    for (const auto& [pair, points] : it->second.bonePositions) {
                        drawList->AddLine(points.first, points.second, color, 1.5f);
                    }
                }
            }
        }
        catch (const std::exception& e) {
            printf("Error in DrawMultipleBoneConnections: %s\n", e.what());
        }
    }

    float GetCurrentFPS(int entityId) const {
        auto it = entityCache.find(entityId);
        return (it != entityCache.end()) ? it->second.currentFPS : 0.0f;
    }

    void ClearBoneCache(int entityId) {
        entityCache.erase(entityId);
    }

    void ClearAllBoneCache() {
        entityCache.clear();
    }

    void SetUpdateInterval(int entityId, float interval) {
        entityCache[entityId].updateInterval = interval;
    }

    void SetDefaultUpdateInterval(float interval) {
        defaultUpdateInterval = interval;
    }
};