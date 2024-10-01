#pragma once

class Hashtable : public std::unordered_map<void*, void*> {};

struct Vector3 {
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Quaternion {
    float x, y, z, w;

    Quaternion() : x(0), y(0), z(0), w(1) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

namespace ExitGames
{
    namespace Client {
        namespace Photon {
            struct EventData {
                uint8_t Code;
                std::shared_ptr<void*> Parameters;
                uint8_t SenderKey;
            };

            struct OperationResponse {
                uint8_t OperationCode;
                int16_t ReturnCode;
                II::String* DebugMessage;
                std::shared_ptr<void*> Parameters;
            };
        };
    };
};

namespace Photon {
    namespace Realtime {
        struct WebFlags {
            static const uint8_t HttpForwardConst = 1;
            static const uint8_t SendAuthCookieConst = 2;
            static const uint8_t SendSyncConst = 4;
            static const uint8_t SendStateConst = 8;

            static const WebFlags Default;

            uint8_t WebhookFlags;

        };
        struct PhotonPortDefinition {
            static const PhotonPortDefinition AlternativeUdpPorts;
            uint16_t NameServerPort;    // Port for Name Server
            uint16_t MasterServerPort;   // Port for Master Server
            uint16_t GameServerPort;     // Port for Game Server
        };

        struct Room {
            void* LoadBalancingClient; // Pointer to LoadBalancingClient
            bool isOffline;                                           // Offline status
            std::unordered_map<int, void*> players; // Dictionary mapping player IDs to Player pointers
            bool BroadcastPropertiesChangeToAll;                    // Backing field
            bool SuppressRoomEvents;                                 // Backing field
            bool SuppressPlayerInfo;                                 // Backing field
            bool PublishUserId;                                      // Backing field
            bool DeleteNullProperties;                               // Backing field
        };
        struct Player {
            Room* RoomReference; // Pointer to Room, equivalent to backing field
            int actorNumber;                         // Actor number
            const bool IsLocal;                      // Readonly field, could be const or marked appropriately in the constructor
            bool HasRejoined;                        // Backing field
            II::String* nickName;                   // Nickname
            II::String* UserId;                     // Backing field
            bool IsInactive;                         // Backing field
            Hashtable* CustomProperties; // Pointer to Hashtable
            void* TagObject;                         // Pointer to any object (void* for flexibility)
        };

        struct TypedLobby {
            II::String* Name;
            LobbyType Type;
            static const TypedLobby Default;
        };

        struct LoadBalancingClient {
            void* loadBalancingPeer = nullptr;
            II::String* appVersion;
            II::String* appId;
            ClientAppType clientType;
            AuthModeOption authMode;
            EncryptionMode encryptionMode;
            ConnectionProtocol expectedProtocol;
            II::String* nameServerHost;
            Photon::Realtime::PhotonPortDefinition serverPortOverrides;
            DisconnectCause disconnectedCause;
            bool isUsingNameServer = false;
            bool enableLobbyStatistics = false;

            void* stateChangedCallback;
            void* eventReceivedCallback;
            void* opResponseReceivedCallback;
            //std::function<void(ClientState, ClientState)> stateChangedCallback;
            //std::function<void(EventData)> eventReceivedCallback;
            //std::function<void(OperationResponse)> opResponseReceivedCallback;

            std::vector<TypedLobby> lobbyStatistics;

            Photon::Realtime::Player* localPlayer = nullptr;
            Photon::Realtime::Room* currentRoom = nullptr;
        };

        struct RaiseEventOptions {
            static RaiseEventOptions Default;

            EventCaching CachingOption;
            uint8_t InterestGroup;
            std::vector<int> TargetActors;
            ReceiverGroup Receivers;
            uint8_t SequenceChannel;
            WebFlags Flags;
        };
    };

    namespace Pun {
        struct PhotonRigidbodyView {
            float m_Distance;
            float m_Angle;
            void* m_Body;
            Vector3 m_NetworkPosition;
            Quaternion m_NetworkRotation;
            bool m_SynchronizeVelocity;
            bool m_SynchronizeAngularVelocity;
            bool m_TeleportEnabled;
            float m_TeleportIfDistanceGreaterThan;
        };

        struct PhotonView {
            // Fields
            uint8_t Group; // Equivalent to byte in C#
            int32_t prefixField; // Equivalent to int in C#
            void** instantiationDataField; // Equivalent to object[] in C#
            std::vector<void*> lastOnSerializeDataSent; // Equivalent to List<object> in C#
            std::vector<void*> syncValues; // Equivalent to List<object> in C#
            void** lastOnSerializeDataReceived; // Equivalent to object[] in C#
            ViewSynchronization Synchronization; // Equivalent enum
            bool mixedModeIsReliable; // Equivalent to bool in C#
            OwnershipOption OwnershipTransfer; // Equivalent enum
            enum class ObservableSearch { Manual, AutoFindActive, AutoFindAll }; // Equivalent enum
            ObservableSearch observableSearch; // Equivalent enum
            std::vector<II::MonoBehaviour*> ObservedComponents; // List of observed components
            II::MonoBehaviour** RpcMonoBehaviours; // Equivalent to MonoBehaviour[]
            bool IsMine; // Backing field for IsMine property
            Photon::Realtime::Player* Controller; // Backing field for Controller property
            int32_t CreatorActorNr; // Backing field for CreatorActorNr property
            bool AmOwner; // Backing field for AmOwner property
            Photon::Realtime::Player* Owner; // Backing field for Owner property
            int32_t ownerActorNr; // Non-serialized field
            int32_t controllerActorNr; // Non-serialized field
            int32_t sceneViewId; // Serialized and hidden field
            int32_t viewIdField; // Non-serialized field
            int32_t InstantiationId; // Equivalent to int in C#
            bool isRuntimeInstantiated; // Serialized and hidden field
            bool removedFromLocalViewList; // Backing field
            void* otherDataSkip;
            //std::queue<CallbackTargetChange> CallbackChangeQueue; // Queue for callback changes
           // std::vector<IOnPhotonViewPreNetDestroy*> OnPreNetDestroyCallbacks; // List of callbacks
            //std::vector<IOnPhotonViewOwnerChange*> OnOwnerChangeCallbacks; // List of callbacks
           // std::vector<IOnPhotonViewControllerChange*> OnControllerChangeCallbacks; // List of callbacks
        };

        struct PhotonMessageInfo {
            int64_t timestamp;
            Photon::Realtime::Player* sender;
            PhotonView* photonView;
        };
    };

    
};

//struct CallbackTargetChange {
  //  IPhotonViewCallback* obj; // Pointer to the callback object
    //std::type_index type; // Type of the callback
    //bool add; // Flag for addition
//};

class LevelRoom : public II::MonoBehaviour {
public:
    void* data;
    //inline static std::vector<LevelRoom*> rooms{};
    //inline static std::mutex         roomsMutex;
};

class SceneManager : public II::MonoBehaviour {
public:
    void* data;
};

class GameController : public II::MonoBehaviour {
public:
    void* data;
};

class RewardManager : public II::MonoBehaviour {
public:
    void* data;
};

class MainManager : public II::MonoBehaviour {
public:
    void* data;
};

class JournalController : public II::MonoBehaviour {
public:
    void* data;
};

class LevelController : public II::MonoBehaviour {
public:
    void* data;
};

class Player : public II::MonoBehaviour {
public:
    void* data;
};


// ghost
struct GhostTraits {
    GhostType typeFirst;
    GhostType typeSecond;
    II::List<Proof>* proofFirst;
    II::List<Proof>* proofSecond;
    int age;
    bool isMale;
    II::String* name;
    int ghostFirstNameID;
    int ghostLastNameID;
    bool isShy;
    int deathLength;
    int favouriteRoomID;
    bool unk;
};

class GhostInfo : public II::MonoBehaviourPun {
public:
    GhostTraits traits; // 0x28
private:
    void* ghostAI; // 0x68
public:
    void* ghostRoom; // 0x70
    float fltUnk; // 0x78
    bool blUnk; // 0x7C
};

class GhostAI : public II::MonoBehaviour {
public:
    void* data;
    /*char padding1[8];
    GhostState state; // 0x28
    Photon::Pun::PhotonView photonView; // 0x30
    GhostInfo ghostInfo; // 0x38
    void* navMeshAgent; // 0x40
    void* ghostAudio; // 0x48
    void* ghostInteraction; // 0x50
    void* ghostActivity; // 0x58
    void* ghostModel; // 0x60
    void* eventModel; // 0x68
    void** modelTmp; // 0x70
    void** modelTmp2; // 0x78
    bool unkbool; // 0x80
    void* shadowCastingMode; // 0x84
    II::List<II::Vector3>* position; // 0x88
    float unk; // private: 0x90
    void* sanityDrainer; // 0x98
    bool unkbool2; // 0xA0
    void* layerMask; // 0xA4
    II::Transform* pTransformFirst; // 0xA8
    II::Transform* pTransformSecond; // 0xB0
    II::Transform* pTransformThree; // 0xB8
    float defaultSpeed; // 0xC0
    float fltunk2;  // 0xC4
    float fltunk3; // 0xC8
    bool unkbool4; // 0xCC
    bool unkbool5; // 0xCD
    II::Vector3* unkVec; //0xD0
    II::GameObject* pGameObj; // 0xE0
    bool canAttack; // 0xE8
    bool isHunting; // 0xE9 ??????????
    bool smudgeSticksUsed; // 0xEA;
    bool canWander; // 0xEB
     
    char skipTo[0x15];

    void* bansheeTarget;
    char skipTo2[0x80];*/
};

class EMF : public II::MonoBehaviour {
public:
    char padding[0x30]; // Skip to offset 0x30, which is where ghostAction starts
    EMFGhostAction ghostAction;
    EMFGhostActionType ghostActionType;
    float placeholder_3;
};

class DNAEvidence : public II::MonoBehaviour {
public:
    void* pData;
};