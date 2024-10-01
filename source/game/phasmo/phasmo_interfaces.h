#pragma once

class IPhotonViewCallback {
public:
    virtual ~IPhotonViewCallback() = default;
};
class IOnPhotonViewPreNetDestroy : public IPhotonViewCallback {
public:
    // Destructor
    virtual ~IOnPhotonViewPreNetDestroy() = default;

    // Method to be implemented by derived classes
    virtual void OnPreNetDestroy(void* rootView) = 0; // Use pointer for polymorphism
};
class IOnPhotonViewOwnerChange : public IPhotonViewCallback {
public:
    // Destructor
    virtual ~IOnPhotonViewOwnerChange() = default;

    // Method to be implemented by derived classes
    virtual void OnOwnerChange(void* newOwner, void* previousOwner) = 0; // Use pointers for polymorphism
};
class IOnPhotonViewControllerChange : public IPhotonViewCallback {
public:
    // Destructor
    virtual ~IOnPhotonViewControllerChange() = default;

    // Method to be implemented by derived classes
    virtual void OnControllerChange(void* newController, void* previousController) = 0; // Use pointers for polymorphism
};