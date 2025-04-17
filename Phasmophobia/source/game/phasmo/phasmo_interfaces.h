#pragma once

class IPhotonViewCallback {
public:
    virtual ~IPhotonViewCallback() = default;
};
class IOnPhotonViewPreNetDestroy : public IPhotonViewCallback {
public:
    virtual ~IOnPhotonViewPreNetDestroy() = default;
    virtual void OnPreNetDestroy(void* rootView) = 0; 
};
class IOnPhotonViewOwnerChange : public IPhotonViewCallback {
public:
    virtual ~IOnPhotonViewOwnerChange() = default;
    virtual void OnOwnerChange(void* newOwner, void* previousOwner) = 0;
};
class IOnPhotonViewControllerChange : public IPhotonViewCallback {
public:
    virtual ~IOnPhotonViewControllerChange() = default;
    virtual void OnControllerChange(void* newController, void* previousController) = 0; 
};