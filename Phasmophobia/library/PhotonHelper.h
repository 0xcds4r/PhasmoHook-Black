#pragma once

namespace Photon {
    namespace Realtime {
        struct __declspec(align(8)) Player {
            void* _RoomReference_k__BackingField;
            int32_t actorNumber;           
            bool isLocal;                  
            bool _HasRejoined_k__BackingField;
            II::String* nickName;          
            II::String* _UserId_k__BackingField;
            bool _IsInactive_k__BackingField;
            void* _CustomProperties_k__BackingField;              
            void* TagObject;

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
                    return T();  
                }
        };
        VALIDATE_SIZE(Player, 0x38);
    };

    namespace Pun 
    {
        class PhotonNetwork 
        {
        public:
            // TODO: PhotonNetwork
            static void* GetLocalPlayer() {
                return InvokeMethod<void*>("get_LocalPlayer");
            }

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
            uint8_t group;                         
            int32_t prefixField;                
            II::List<void*>* instantiationDataField;  
            II::List<void*>* lastOnSerializeDataSent;          
            II::List<void*>* syncValues;   
            II::List<void*>* lastOnSerializeDataReceived;
            ViewSynchronization synchronization;    
            bool mixedModeIsReliable;               
            OwnershipOption ownershipTransfer;      
            ObservableSearch observableSearch;     
            II::List<void*>* observedComponents; 
            II::List<II::MonoBehaviour*>* rpcMonoBehaviours;               
            bool _IsMine_k__BackingField;
            void* IsMine;
            int32_t _CreatorActorNr_k__BackingField; 
            bool _AmOwner_k__BackingField;
            void* Owner;
            int32_t ownerActorNr;
            int32_t controllerActorNr;              
            int32_t sceneViewId;                   
            int32_t viewIdField;                    
            int32_t instantiationId;                
            bool isRuntimeInstantiated;             
            bool removedFromLocalViewList;         
            void* CallbackChangeQueue;              
            void* OnPreNetDestroyCallbacks;         
            void* OnOwnerChangeCallbacks;           
            void* OnControllerChangeCallbacks;     

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
        VALIDATE_SIZE(PhotonView, 0xB0 + STRUCT_STUCK);
    } // namespace Pun
} // namespace Photon