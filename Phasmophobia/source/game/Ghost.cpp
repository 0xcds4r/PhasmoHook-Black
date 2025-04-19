#include "../main.h"
#include "Game.h"

void Ghost::Init()
{
    LOGD("Ghost::Init");
    Ghost::Reset();
}

void Ghost::Reset() {
    gCurrentGhost = nullptr;
    bansheeTarget = nullptr;
    Ghost::emfData.clear();
}

const char* GetGhostTypeString(GhostTraits::GhostType type) {
    auto it = GhostTypeNames.find(type);
    return (it != GhostTypeNames.end()) ? it->second : "Unknown";
}

const char* GetGhostTypeStringRus(GhostTraits::GhostType type) {
    auto it = GhostTypeNamesRus.find(type);
    return (it != GhostTypeNamesRus.end()) ? it->second : "Неизвестный";
}

const char* GetGhostStateString(GhostAI::States state) {
    auto it = GhostStateNames.find(state);
    return (it != GhostStateNames.end()) ? it->second : "Unknown";
}

const char* GetEMFGhostActionNames(EMFGhostActionEv action) {
    auto it = EMFGhostActionNames.find(action);
    return (it != EMFGhostActionNames.end()) ? it->second : "Unknown";
}

const char* GetEMFGhostActionTypeNames(EMFGhostActionType action) {
    auto it = EMFGhostActionTypeNames.find(action);
    return (it != EMFGhostActionTypeNames.end()) ? it->second : "Unknown";
}

GhostAI::States Ghost::GetState(GhostAI* _this) {
    if (!_this) return GhostAI::States::idle;
    return _this->currentState;
}

void* Ghost::GetGhostModel(GhostAI* _this) {
    if (!_this) return NULL;
    return _this->currentGhostModel;
}

GhostInfo* Ghost::GetInfo(GhostAI* _this) {
    if (!_this) return nullptr;
    return _this->ghostInfo;
    //return GetOffsetValue<GhostInfo*>((void*)_this, 0x38, "Assembly-CSharp.dll", "GhostAI");
}

bool Ghost::IncenseEffect() {
    return Ghost::gCurrentGhost->delayedBySmudgeStick; // when using incense it sets to 1
}

const char* Ghost::GetBansheeTargetNickname()
{
    if (Ghost::GetType() != GhostTraits::GhostType::Banshee) return "";
    if (!bansheeTarget) bansheeTarget = Ghost::gCurrentGhost->bansheeTarget;
    if (!bansheeTarget) return "";

    return Players::GetNickname(bansheeTarget).c_str();
}

bool Ghost::IsGhostEvent() {
    return Ghost::GetState(Ghost::gCurrentGhost) == GhostAI::States::randomEvent; // new offset
}

bool Ghost::IsHunting() {
    return Ghost::gCurrentGhost->isHunting;
}

void Ghost::SetNewBansheeTarget(Player* player)
{
    bansheeTarget = player;
}

const char* Ghost::GetName()
{
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo && pInfo->ghostTraits.GhostName) {
            return pInfo->ghostTraits.GhostName->ToString().c_str();
        }
    }
    return "None";
}

GhostTraits::GhostType Ghost::GetType() {
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->ghostTraits.ghostType;
        }
    }

    return GhostTraits::GhostType::Spirit;
}

GhostTraits::GhostType Ghost::GetMimicType() 
{
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->ghostTraits.mimicType;
        }
    }

    return GhostTraits::GhostType::Spirit;
}

const char* Ghost::GetTypeName(GhostTraits::GhostType type, bool bRussianLocale)
{
    if (Ghost::gCurrentGhost) {
        return bRussianLocale ? GetGhostTypeStringRus(type) : GetGhostTypeString(type);
    }
    return "Loading..";
}

//const char* Ghost::GetTypeNameRus()
//{
//    if (Ghost::gCurrentGhost) {
//        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
//        if (pInfo) {
//            auto type = pInfo->ghostTraits.ghostType;
//            return GetGhostTypeStringRus(type);
//        }
//    }
//    return "None";
//}

int Ghost::GetAge() {
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->ghostTraits.ghostAge;
        }
    }
    return -1;
}

const char* Ghost::GetDescription(bool bRussian)
{
    // бла бла бла, лень расписывать, но надо доделать для легита, ибо долбоебы спросят за ''идеальную игру''
    auto getGhostDesc = [](const std::string& ghostName, const std::string& lang) -> const char*
        {
            std::map<std::string, std::pair<std::string, std::string>> ghostDescriptions = {
                {"Spirit", {
                    "Passive, rarely hunts without provocation. Speed: 1.7 m/s (standard). Flicker: standard (0.3-0.5s visible, 0.1-0.3s invisible). Unique Ability: Can be angered by smudging, stopping hunts for longer (90s). Trait: Minimal interaction with objects unless provoked. Reacts to salt but doesn’t avoid it.",
                    "Пассивен, редко охотится без провокаций. Скорость: 1.7 м/с (стандарт). Мерцание: стандартное (0.3-0.5 сек. видимость, 0.1-0.3 сек. невидимость). Уникальная способность: Злится на благовония, что останавливает охоту дольше (90 сек). Особенность: Минимальное взаимодействие с объектами без провокации. Реагирует на соль, но не избегает её."
                }},
                {"Wraith", {
                    "Floats, no footsteps heard. Speed: 1.7 m/s (standard). Flicker: standard. Unique Ability: Can teleport to a random player, causing activity spikes. Trait: Avoids salt entirely, never stepping in it. Passes through walls, emits rustling or breathing sounds.",
                    "Летает, шагов не слышно. Скорость: 1.7 м/с (стандарт). Мерцание: стандартное. Уникальная способность: Может телепортироваться к случайному игроку, вызывая всплеск активности. Особенность: Полностью избегает соль, никогда не наступая на неё. Проходит сквозь стены, издает шорох или дыхание."
                }},
                {"Phantom", {
                    "Disappears when photographed, heavily drains sanity on sight. Speed: 1.7 m/s. Flicker: more invisible (0.1-0.3s visible, 0.3-0.5s invisible). Unique Ability: Rarely appears in full form during hunts, often vanishing. Trait: Causes stronger sanity drops during manifestations. Appears as a mirage.",
                    "Исчезает при фото, сильно снижает рассудок при взгляде. Скорость: 1.7 м/с. Мерцание: чаще невидим (0.1-0.3 сек. видимость, 0.3-0.5 сек. невидимость). Уникальная способность: Редко появляется в полной форме во время охоты, часто исчезает. Особенность: Вызывает более сильное снижение рассудка при проявлениях. Появляется как мираж."
                }},
                {"Poltergeist", {
                    "Throws objects, loud crashing sounds. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Can throw multiple objects at once, creating a chaotic mess. Trait: Loses strength in empty rooms with no objects. Activity increases with people.",
                    "Бросает предметы, громкие звуки падения. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Может бросать несколько предметов одновременно, создавая хаос. Особенность: Теряет силу в пустых комнатах без объектов. Активность растет с людьми."
                }},
                {"Banshee", {
                    "Targets one player, piercing scream. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Ignores other players during hunts, focusing solely on its target. Trait: Roams toward its target more often when not hunting. May sing or wail.",
                    "Фиксируется на одном игроке, пронзительный крик. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Игнорирует других игроков во время охоты, сосредотачиваясь только на цели. Особенность: Чаще бродит к своей цели, когда не охотится. Может петь или выть."
                }},
                {"Jinn", {
                    "Speeds up with power on (up to 2.5 m/s), else 1.7 m/s. Flicker: standard. Unique Ability: Can instantly drain sanity when near active electronics. Trait: Rarely turns off the breaker unless provoked. Manipulates lights, emits hum.",
                    "Ускоряется при включенном щитке (до 2.5 м/с), иначе 1.7 м/с. Мерцание: стандартное. Уникальная способность: Может мгновенно снижать рассудок рядом с активной электроникой. Особенность: Редко выключает щиток без провокации. Манипулирует светом, издает гул."
                }},
                {"Mare", {
                    "Loves darkness, turns off lights. Speed: 1.7 m/s, faster in dark (up to 2 m/s). Flicker: standard. Unique Ability: Hunts more often when lights are off, avoiding lit areas. Trait: Rarely interacts in well-lit rooms. Quiet moans.",
                    "Любит темноту, выключает свет. Скорость: 1.7 м/с, ускоряется в темноте (до 2 м/с). Мерцание: стандартное. Уникальная способность: Чаще охотится при выключенном свете, избегая освещенных зон. Особенность: Редко взаимодействует в хорошо освещенных комнатах. Тихие стоны."
                }},
                {"Revenant", {
                    "Slow without sight (1 m/s), speeds up when chasing (3 m/s). Flicker: standard. Unique Ability: Switches speed dramatically based on line of sight. Trait: Relentless pursuit once it spots a player. Loud footsteps during hunt.",
                    "Медленный без видимости (1 м/с), ускоряется при погоне (3 м/с). Мерцание: стандартное. Уникальная способность: Резко меняет скорость в зависимости от видимости. Особенность: Неустанное преследование, если заметит игрока. Громкие шаги при охоте."
                }},
                {"Shade", {
                    "Passive with groups, active alone. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Rarely interacts when multiple players are present. Trait: Prefers subtle interactions like whispers or shadows. Quiet whispers.",
                    "Пассивен при группе, активен в одиночестве. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Редко взаимодействует при наличии нескольких игроков. Особенность: Предпочитает тонкие взаимодействия, такие как шепот или тени. Тихий шепот."
                }},
                {"Demon", {
                    "Aggressive, frequent hunts. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Can hunt at higher sanity levels (up to 70%). Trait: Smudging only delays hunts briefly (60s). Growls, crucifix works only up close.",
                    "Агрессивен, частые охоты. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Может охотиться при высоком рассудке (до 70%). Особенность: Благовония лишь ненадолго откладывают охоту (60 сек). Рычание, распятие работает только вблизи."
                }},
                {"Yurei", {
                    "Heavily drains sanity, moves doors. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Can trap players by shutting doors more often. Trait: Smudging confines it to its room for longer (90s). Leaves activity traces.",
                    "Сильно снижает рассудок, двигает двери. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Может запирать игроков, чаще закрывая двери. Особенность: Благовония дольше удерживают его в комнате (90 сек). Следы активности в комнате."
                }},
                {"Oni", {
                    "Active with groups, moves objects. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Drains sanity faster during manifestations. Trait: Often appears in full form, rarely as a shadow. Loud sounds (growling).",
                    "Активен при группе, передвигает предметы. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Быстрее снижает рассудок при проявлениях. Особенность: Часто появляется в полной форме, редко как тень. Громкие звуки (рычание)."
                }},
                {"Yokai", {
                    "Reacts to voices, aggressive when spoken to. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Hears voices from a shorter distance, triggering hunts. Trait: Less active when players are silent. Quiet whispers.",
                    "Реагирует на голоса, агрессивен при разговорах. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Слышит голоса с меньшего расстояния, начиная охоту. Особенность: Менее активен, когда игроки молчат. Тихий шепот."
                }},
                {"Hantu", {
                    "Faster in cold (up to 2.7 m/s), slower in warmth (1.4 m/s). Flicker: standard. Unique Ability: Leaves visible breath in cold areas during hunts. Trait: Avoids warmer areas, sticking to colder zones. Breath visible, breathing sound.",
                    "Быстрее в холоде (до 2.7 м/с), медленнее в тепле (1.4 м/с). Мерцание: стандартное. Уникальная способность: Оставляет видимый пар от дыхания в холодных зонах во время охоты. Особенность: Избегает теплых зон, предпочитая холодные. Пар изо рта, звук дыхания."
                }},
                {"Goryo", {
                    "Stays in its room. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Only manifests fully when no players are nearby. Trait: Rarely roams far from its haunt spot. Rare physical manifestations.",
                    "Привязан к своей комнате. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Полностью проявляется, только если игроков нет рядом. Особенность: Редко уходит далеко от места обитания. Редкие физические проявления."
                }},
                {"Myling", {
                    "Quiet footsteps(heard close).Speed: 1.7 m / s.Flicker : standard.Unique Ability : Sounds fade out quickly, audible only at close range.Trait : Silent until very close during hunts.Quiet moans before attack.",
                    "Тихие шаги (слышны вблизи). Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Звуки быстро затухают, слышны только вблизи. Особенность: Молчит, пока не подойдет очень близко во время охоты. Тихие стоны перед атакой."
                }},
                {"Onryo", {
                    "Blows out candles, hunts with fire. Speed: 1.7 m/s. Flicker: standard. Unique Ability: Extinguishing a flame can trigger an immediate hunt. Trait: More active around lit candles before extinguishing them. Crying or wind sounds.",
                    "Задувает свечи, охотится при огне. Скорость: 1.7 м/с. Мерцание: стандартное. Уникальная способность: Тушение пламени может мгновенно начать охоту. Особенность: Более активен около зажженных свечей перед их тушением. Плач или звук ветра."
                }},
                {"TheTwins", {
                    "Variable speed (1.5 m/s or 1.9 m/s). Flicker: standard. Unique Ability: Can alternate between two entities, one hunting while the other interacts. Trait: Interactions may occur far from the hunting twin. Dual sounds (steps, knocks).",
                    "Переменная скорость (1.5 м/с или 1.9 м/с). Мерцание: стандартное. Уникальная способность: Может чередоваться между двумя сущностями, одна охотится, другая взаимодействует. Особенность: Взаимодействия могут происходить далеко от охотящегося близнеца. Двойные звуки (шаги, стуки)."
                }},
                {"Raiju", {
                    "Speeds up near electronics (up to 2.5 m/s), else 1.7 m/s. Flicker: standard. Unique Ability: Disrupts electronic equipment from a greater distance. Trait: Hunts more often when players use electronics. Crackling or humming.",
                    "Ускоряется у электроники (до 2.5 м/с), иначе 1.7 м/с. Мерцание: стандартное. Уникальная способность: Нарушает работу электроники с большего расстояния. Особенность: Чаще охотится, когда игроки используют электронику. Треск или гудение."
                }},
                {"Obake", {
                    "Shape-shifts, unpredictable. Speed: 1.7 m/s. Flicker: may be irregular (random intervals). Unique Ability: Can briefly mimic a human form or leave unique traces (e.g., six-fingered prints). Trait: Changes appearance more often during manifestations. Giggling.",
                    "Меняет форму, непредсказуем. Скорость: 1.7 м/с. Мерцание: может быть нестандартным (случайные интервалы). Уникальная способность: Может ненадолго принимать человеческий вид или оставлять уникальные следы (например, шестипалые отпечатки). Особенность: Чаще меняет внешний вид при проявлениях. Хихиканье."
                }},
                {"Mimic", {
                    "Mimics others (speed varies: 1.5-2.5 m/s). Flicker: depends on imitation. Unique Ability: Can replicate the behavior of any other ghost type. Trait: May switch behaviors mid-investigation. Mixed sounds.",
                    "Подражает другим (скорость варьируется: 1.5-2.5 м/с). Мерцание: зависит от имитации. Уникальная способность: Может копировать поведение любого другого типа призрака. Особенность: Может менять поведение в середине расследования. Смешанные звуки."
                }},
                {"Moroi", {
                    "Speeds up with low sanity (1.7-2.5 m/s). Flicker: standard. Unique Ability: Curses players, causing faster sanity drain over time. Trait: Becomes more vocal as sanity drops. Whispers in mic.",
                    "Ускоряется при низком рассудке (1.7-2.5 м/с). Мерцание: стандартное. Уникальная способность: Проклинает игроков, ускоряя снижение рассудка со временем. Особенность: Становится более голосистым при падении рассудка. Шепот в микрофон."
                }},
                {"Deogen", {
                    "Knows player location, slows near prey (1.7 m/s to 0.4 m/s). Flicker: longer visibility (0.5-0.7s). Unique Ability: Always tracks players, making hiding ineffective. Trait: Emits louder breathing when close to players. Heavy breathing.",
                    "Знает местоположение, замедляется вблизи (1.7 м/с до 0.4 м/с). Мерцание: длиннее видимость (0.5-0.7 сек.). Уникальная способность: Всегда отслеживает игроков, делая укрытия бесполезными. Особенность: Издает громкое дыхание, когда близко к игрокам. Тяжелое дыхание."
                }},
                {"Thaye", {
                    "Speed changes over time (2.5 m/s early, 1 m/s later). Flicker: standard. Unique Ability: Ages over time, becoming less aggressive as the investigation progresses. Trait: More frequent interactions early in the game. Elderly moans.",
                    "Меняет скорость со временем (2.5 м/с в начале, 1 м/с позже). Мерцание: стандартное. Уникальная способность: Стареет со временем, становясь менее агрессивным по мере расследования. Особенность: Более частые взаимодействия в начале игры. Старческие стоны."
                }}
            };

            std::string description = ghostDescriptions.count(ghostName)
                ? (lang == "eng" ? ghostDescriptions[ghostName].first : ghostDescriptions[ghostName].second)
                : (lang == "eng" ? "Unknown ghost: No data on behavior, speed, or flicker." : "Неизвестный призрак: Нет данных о поведении, скорости или мерцании.");

            return description.c_str();
        };

    return getGhostDesc(Ghost::GetTypeName(), bRussian ? "rus" : "eng");
}

const char* Ghost::GetSex() {
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->ghostTraits.isMale ? "Male" : "Female";
        }
    }
    return "None";
}

int Ghost::GetFavouriteRoomID() {
    if (Ghost::gCurrentGhost) {
        auto pInfo = Ghost::GetInfo(Ghost::gCurrentGhost);
        if (pInfo) {
            return pInfo->ghostTraits.favouriteRoomID;
        }
    }
    return -1;
}

const char* Ghost::GetStateName() {
    if (Ghost::gCurrentGhost) {
        auto state = Ghost::GetState(Ghost::gCurrentGhost);
        return GetGhostStateString(state);
    }
    return "None";
}

