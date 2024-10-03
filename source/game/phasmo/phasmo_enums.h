#pragma once
#include <cstdint>

enum ClientAppType
{
    Realtime,
    Voice,
    Fusion
};

enum AuthModeOption
{
    Auth,
    AuthOnce,
    AuthOnceWss
};

enum EncryptionMode
{
    PayloadEncryption,
    DatagramEncryption = 10,
    DatagramEncryptionRandomSequence,
    DatagramEncryptionGCM = 13
};

enum ConnectionProtocol : char
{
    Udp,
    Tcp,
    WebSocket = 4,
    WebSocketSecure
};

enum class DisconnectCause {
    None = 0,
    ExceptionOnConnect = 1,
    DnsExceptionOnConnect = 2,
    ServerAddressInvalid = 3,
    Exception = 4,
    ServerTimeout = 5,
    ClientTimeout = 6,
    DisconnectByServerLogic = 7,
    DisconnectByServerReasonUnknown = 8,
    InvalidAuthentication = 9,
    CustomAuthenticationFailed = 10,
    AuthenticationTicketExpired = 11,
    MaxCcuReached = 12,
    InvalidRegion = 13,
    OperationNotAllowedInCurrentState = 14,
    DisconnectByClientLogic = 15,
    DisconnectByOperationLimit = 16,
    DisconnectByDisconnectMessage = 17,
    ApplicationQuit = 18
};

enum class LobbyType : char {
    Default,
    SqlLobby = 2,
    AsyncRandomLobby
};

enum class ClientState {
    PeerCreated = 0,
    Authenticating = 1,
    Authenticated = 2,
    JoiningLobby = 3,
    JoinedLobby = 4,
    DisconnectingFromMasterServer = 5,
    DisconnectingFromMasterserver = DisconnectingFromMasterServer, // Obsolete, but kept for compatibility
    ConnectingToGameServer = 6,
    ConnectingToGameserver = ConnectingToGameServer, // Obsolete, but kept for compatibility
    ConnectedToGameServer = 7,
    ConnectedToGameserver = ConnectedToGameServer, // Obsolete, but kept for compatibility
    Joining = 8,
    Joined = 9,
    Leaving = 10,
    DisconnectingFromGameServer = 11,
    DisconnectingFromGameserver = DisconnectingFromGameServer, // Obsolete, but kept for compatibility
    ConnectingToMasterServer = 12,
    ConnectingToMasterserver = ConnectingToMasterServer, // Obsolete, but kept for compatibility
    Disconnecting = 13,
    Disconnected = 14,
    ConnectedToMasterServer = 15,
    ConnectedToMasterserver = ConnectedToMasterServer, // Obsolete, but kept for compatibility
    ConnectedToMaster = ConnectedToMasterServer, // Obsolete, but kept for compatibility
    ConnectingToNameServer = 16,
    ConnectedToNameServer = 17,
    DisconnectingFromNameServer = 18,
    ConnectWithFallbackProtocol = 19
};

enum class EventCaching : uint8_t {
    DoNotCache = 0,                     // not cache
    MergeCache,                         // outdated
    ReplaceCache,                       // outdated
    RemoveCache,                        // outdated
    AddToRoomCache,                     // add to room cache
    AddToRoomCacheGlobal,               // add global room cache
    RemoveFromRoomCache,                // delete from room cache
    RemoveFromRoomCacheForActorsLeft,   // delete from cache left players
    SliceIncreaseIndex = 10,            // increase index of Slice
    SliceSetIndex,                      // setup index of Slice
    SlicePurgeIndex,                    // clear slice by index
    SlicePurgeUpToIndex                 // clear all slice to index
};

enum class ReceiverGroup : uint8_t {
    Others = 0,         // other
    All,                // all
    MasterClient        // main client
};

enum class ViewSynchronization {
    Off,                            // 0
    ReliableDeltaCompressed,       // 1
    Unreliable,                    // 2
    UnreliableOnChange             // 3
};

enum class OwnershipOption {
    Fixed,      // 0
    Takeover,   // 1
    Request     // 2
};

// ghost

enum class GhostState : std::int64_t {
    Idle,
    Wander,
    Hunting,
    FavouriteRoom,
    Light,
    Door,
    Throwing,
    Fusebox,
    Appear,
    DoorKnock,
    WindowKnock,
    CarAlarm,
    Flicker,
    Cctv,
    RandomEvent,
    GhostAbility,
    Mannequin,
    TeleportObject,
    Interact,
    SummoningCircle,
    MusicBox,
    Dots,
    Salt
};

enum class GhostType : std::int32_t {
    Spirit = 1,
    Wraith,
    Phantom,
    Poltergeist,
    Banshee,
    Jinn,
    Mare,
    Revenant,
    Shade,
    Demon,
    Yurei,
    Oni,
    Yokai,
    Hantu,
    Goryo,
    Myling,
    Onryo,
    TheTwins,
    Raiju,
    Obake,
    Mimic,
    Moroi,
    Deogen,
    Thaye
};

enum class Proof : std::int32_t {
    None,
    Emf,
    SpiritBox,
    UltraViolet,
    GhostOrb,
    GhostWritingBook,
    Temperature,
    DotsProjector
};

enum class EMFGhostActionEv {
    GhostInteraction,   // (2) Ghost Interaction
    GhostThrowing,      // (3) Ghost Throwing
    GhostAppeared,      // (4) Ghost Appeared
    GhostEvidence       // (5) Ghost Evidence - DO NOT USE
};

enum class EMFGhostActionType {
    interaction,
    objectMoved,
    windowKnock,
    midGhostWrite,
    breakerSwitch,
    planchette,
    rockingChair,
    generalIntEffect,
    lightSwitched,
    soundEffect,
    crucifixFire
};

enum class LocationType {
    InsideRoom,     // Represents being inside a room
    OutsideMap,     // Represents being outside the map
    Exterior,       // Represents an exterior location
    ExteriorRoom    // Represents an exterior room
};

enum class FloorType {
    Basement,       // Represents the basement
    FirstFloor,     // Represents the first floor
    SecondFloor,    // Represents the second floor
    ThirdFloor,     // Represents the third floor
    FourthFloor,    // Represents the fourth floor
    FifthFloor,     // Represents the fifth floor
    SixthFloor,     // Represents the sixth floor
    SeventhFloor,   // Represents the seventh floor
    EighthFloor,    // Represents the eighth floor
    NinthFloor,     // Represents the ninth floor
    TenthFloor      // Represents the tenth floor
};
