#pragma once

struct JournalController_Ghost
{
    int32_t type;
    II::String* localisedName;
    int32_t evidence1;
    int32_t evidence2;
    int32_t evidence3;
};
VALIDATE_SIZE(JournalController_Ghost, 0x20);

class Player;

class JournalController : public II::MonoBehaviour {
public:
    void* pauseMenuController;
    void* playerList;
    II::GameObject* content;
    II::GameObject* pauseMenu;
    II::GameObject* journalMenu;
    II::GameObject* recentPlayersMenu;
    II::GameObject* reportPage;
    void* graphicManager;
    void* photoPage;
    void* canvas;
    void* openSource;
    void* openClip;
    void* closeClip;
    void* pauseFlip;
    II::List<void*>* pageTurns;
    bool isOpen;
    bool isVRJournal;
    int32_t ghostPageIndex;
    II::List<II::GameObject*>* pages;
    II::List<void*>* journalTabs;
    II::List<void*>* pauseMenuTabs;
    II::GameObject* left;
    II::GameObject* right;
    void* leftPageNumber;
    void* rightPageNumber;
    int32_t currentPageIndex;
    void* reportInputField;
    void* legacyReportInputField;
    void* view;
    Player* player;
    void* controlsManager;
    void* caseNoText;
    void* ghostNameText;
    void* respondsToText;
    void* mainKeyText;
    void* secondaryKeyText;
    void* mainKeyImage;
    void* secondaryKeyImage;
    void* objective1;
    void* objective2;
    void* objective3;
    II::List<void*>* objectivesComplete;
    II::List<void*>* allGhostTypeButtons;
    II::List<void*>* allEvidenceTypeButtons;
    II::List<void*>* selectedEvidence;
    II::List<void*>* removedEvidence;
    II::List<void*>* correctGhosts;
    II::List<void*>* allGhosts;
    JournalController_Ghost selectedGhost;
    bool hasGhostEvidence;
};
VALIDATE_SIZE(JournalController, 0x1A8 + STRUCT_STUCK);