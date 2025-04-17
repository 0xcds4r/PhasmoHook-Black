#pragma once

// todo
class OuijaBoard : public II::MonoBehaviour {
    void* data;
};

class MusicBox : public II::MonoBehaviour {
    void* data;
};

class TarotCards : public II::MonoBehaviour {
    void* data;
};

class SummoningCircle : public II::MonoBehaviour {
    void* data;
};

class HauntedMirror : public II::MonoBehaviour {
    void* data;
};

class VoodooDoll : public II::MonoBehaviour {
    void* data;
};

class MonkeyPaw : public II::MonoBehaviour {
    void* data;
};

class CursedItemsController : public II::MonoBehaviour
{
public:
    static inline CursedItemsController* instance;
public:
    OuijaBoard* ouijaBoard;
    MusicBox* musicBox;
    TarotCards* tarotCards;
    SummoningCircle* summoningCircle;
    HauntedMirror* hauntedMirror;
    VoodooDoll* voodooDoll;
    MonkeyPaw* monkeyPaw;
    II::Transform* ouijaBoardSpawnSpots;
    II::Transform* musicBoxSpawnSpots;
    II::Transform* tarotCardsSpawnSpots;
    II::Transform* summoningCircleSpawnSpots;
    II::Transform* hauntedMirrorSpawnSpots;
    II::Transform* voodooDollSpawnSpots;
    II::Transform* monkeyPawSpawnSpots;
    void* unk;
};
VALIDATE_SIZE(CursedItemsController, 0x88 + STRUCT_STUCK);