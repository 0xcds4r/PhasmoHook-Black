#pragma once

namespace Photon {
    namespace Realtime {
        struct Player {
            int32_t actorNumber;           // 0x18
            bool isLocal;                  // 0x1C
            II::String* nickName;          // 0x20
            void* tagObject;               // 0x40 (object)

            void* GetRoomReference() const {  // Returns Room*
                return InvokeMethod<void*>("get_RoomReference");
            }

            int32_t GetActorNumber() const {
                return InvokeMethod<int32_t>("get_ActorNumber");
            }

            bool GetHasRejoined() const {
                return InvokeMethod<bool>("get_HasRejoined");
            }

            II::String* GetNickName() const {
                return InvokeMethod<II::String*>("get_NickName");
            }

            II::String* GetUserId() const {
                return InvokeMethod<II::String*>("get_UserId");
            }

            bool GetIsMasterClient() const {
                return InvokeMethod<bool>("get_IsMasterClient");
            }

            bool GetIsInactive() const {
                return InvokeMethod<bool>("get_IsInactive");
            }

            void* GetCustomProperties() const {  // Returns Hashtable*
                return InvokeMethod<void*>("get_CustomProperties");
            }

            private:
                template<typename T>
                T InvokeMethod(const char* methodName) const {
                    UnityResolve::Class* playerClass = UnityResolve::Get("PhotonRealtime.dll")->Get("Player");
                    if (playerClass) {
                        UnityResolve::Method* method = playerClass->Get<UnityResolve::Method>(methodName, {});
                        if (method) {
                            return method->Invoke<T>(this);
                        }
                    }
                    return T();  // Default value on failure
                }
        };
    };

    namespace Pun 
    {
        class PhotonNetwork 
        {
        public:
            // TODO: PhotonNetwork
           /* static II::String* GetGameVersion() {
                return InvokeMethod<II::String*>("get_GameVersion");
            }*/

            private:
                template<typename T>
                static T InvokeMethod(const char* methodName) {
                    UnityResolve::Class* pvClass = UnityResolve::Get("PhotonUnityNetworking.dll")->Get("PhotonNetwork");
                    if (pvClass) {
                        static UnityResolve::Method* method = pvClass->Get<UnityResolve::Method>(methodName, {});
                        if (method) {
                            return method->Invoke<T>();
                        }
                    }
                    return T(); 
                }
        };

        enum class ViewSynchronization : int32_t {
            Off = 0,
            ReliableDeltaCompressed,
            Unreliable,
            UnreliableOnChange
        };

        enum class OwnershipOption : int32_t {
            Fixed = 0,
            Takeover,
            Request
        };

        enum class ObservableSearch : int32_t {
            Manual = 0,
            AutoFindActive,
            AutoFindAll
        };

        struct PhotonView : public II::MonoBehaviour {
            // Fields with their offsets from the C# code
            uint8_t group;                          // 0x20
            int32_t prefixField;                    // 0x24
            void* instantiationDataField;           // 0x28 (object[])
            std::vector<void*> lastOnSerializeDataSent;  // 0x30 (List<object>)
            std::vector<void*> syncValues;          // 0x38 (List<object>)
            void* lastOnSerializeDataReceived;      // 0x40 (object[])
            ViewSynchronization synchronization;    // 0x48
            bool mixedModeIsReliable;               // 0x4C
            OwnershipOption ownershipTransfer;      // 0x50
            ObservableSearch observableSearch;      // 0x54
            std::vector<void*> observedComponents;  // 0x58 (List<Component>)
            void* rpcMonoBehaviours;                // 0x60 (MonoBehaviour[])
            int32_t ownerActorNr;                   // 0x88
            int32_t controllerActorNr;              // 0x8C
            int32_t sceneViewId;                    // 0x90
            int32_t viewIdField;                    // 0x94
            int32_t instantiationId;                // 0x98
            bool isRuntimeInstantiated;             // 0x9C
            bool removedFromLocalViewList;          // 0x9D
            void* callbackChangeQueue;              // 0xA0 (Queue<CallbackTargetChange>)
            void* onPreNetDestroyCallbacks;         // 0xA8 (List<IOnPhotonViewPreNetDestroy>)
            void* onOwnerChangeCallbacks;           // 0xB0 (List<IOnPhotonViewOwnerChange>)
            void* onControllerChangeCallbacks;      // 0xB8 (List<IOnPhotonViewControllerChange>)

            // Property-like methods using UnityResolve
            int32_t GetPrefix() const {
                return InvokeMethod<int32_t>("get_Prefix");
            }

            void* GetInstantiationData() const {
                return InvokeMethod<void*>("get_InstantiationData");
            }

            bool GetIsRoomView() const {
                return InvokeMethod<bool>("get_IsRoomView");
            }

            bool GetIsOwnerActive() const {
                return InvokeMethod<bool>("get_IsOwnerActive");
            }

            bool GetIsMine() const {
                return InvokeMethod<bool>("get_IsMine");
            }

            bool GetAmController() const {
                return InvokeMethod<bool>("get_AmController");
            }

            Photon::Realtime::Player* GetController() const {  // Returns Player*
                return InvokeMethod<Photon::Realtime::Player*>("get_Controller");
            }

            int32_t GetCreatorActorNr() const {
                return InvokeMethod<int32_t>("get_CreatorActorNr");
            }

            bool GetAmOwner() const {
                return InvokeMethod<bool>("get_AmOwner");
            }

            Photon::Realtime::Player* GetOwner() const {  // Returns Player*
                return InvokeMethod<Photon::Realtime::Player*>("get_Owner");
            }

            int32_t GetOwnerActorNr() const {
                return InvokeMethod<int32_t>("get_OwnerActorNr");
            }

            int32_t GetControllerActorNr() const {
                return InvokeMethod<int32_t>("get_ControllerActorNr");
            }

            int32_t GetViewID() const {
                return InvokeMethod<int32_t>("get_ViewID");
            }

        private:
            template<typename T>
            T InvokeMethod(const char* methodName) const {
                UnityResolve::Class* pvClass = UnityResolve::Get("PhotonUnityNetworking.dll")->Get("PhotonView");
                if (pvClass) {
                    UnityResolve::Method* method = pvClass->Get<UnityResolve::Method>(methodName, {});
                    if (method) {
                        return method->Invoke<T>(this);
                    }
                }
                return T();  // Default value on failure
            }
        };
    } // namespace Pun
} // namespace Photon