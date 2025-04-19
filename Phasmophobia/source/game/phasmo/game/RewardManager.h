#pragma once

// 0.12.1.0 [+]

class RewardManager : public II::MonoBehaviourPun {
public:
    void* unlockPopup;
    void* levelUnlocks;
    void* goldTheme;
    void* silverTheme;
    void* bronzeTheme;
    void* lighthouseTheme;
    void* ferrymanTheme;
    void* rangerTheme;
    void* inmateTheme;
    II::Color textOnColor;
    void* audioSource;
    void* successClip;
    void* failClip;
    void* xpLoopClip;
    void* levelUpClip;
    void* mainObjectiveRewardUI;
    II::List<void*>* sideObjectivesRewardUI;
    void* dnaRewardUI;
    void* investigationBonusRewardUI;
    void* perfectBonusRewardUI;
    void* multiplierRewardUI;
    void* negativeMultiplierRewardUI;
    void* diedRewardUI;
    void* photoRewardUI;
    void* bloodMoonRewardUI;
    void* insuranceRewardUI;
    II::List<void*>* dailyRewardUis;
    II::List<void*>* weeklyRewardUis;
    void* eventRewardUI;
    void* totalRewardUI;
    void* ghostTypeText;
    void* playerProfile;
    void* xpGainedText;
    void* nextButton;
    void* skipButton;
    void* summaryButton;
    void* detailedStatsButton;
    void* rewardSequence;
    bool playerDied;
    int32_t totalReward;
    int32_t totalXP;
    int32_t insurance;
    int32_t bloodMoonReward;
    int32_t eventReward;
    bool unlockedTrophyOrBadge;
};
VALIDATE_SIZE(RewardManager, 0x170 + STRUCT_STUCK);
