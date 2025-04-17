#include "../source/main.h"
#include "UnityResolve.hpp"
#include "GameObjectHelper.h"

void SetGameObjectColor(const char* objectName, float r, float g, float b, float a) 
{
    auto gameObject = II::GameObject::Find(objectName);
    if (!gameObject) {
        return;
    }

    auto renderer = gameObject->GetComponent<II::MeshRenderer*>();
    if (!renderer) {
        std::cout << "MeshRenderer not found on '" << objectName << "'!" << std::endl;
        return;
    }

    UnityResolve::UnityType::Material* material = renderer->GetMaterial();
    if (!material) {
        std::cout << "Material not found on '" << objectName << "'!" << std::endl;
        return;
    }

    UnityResolve::UnityType::Color color{ r, g, b, a };
    material->SetColor(color); // "_Color"

    std::cout << "Set color to (" << r << ", " << g << ", " << b << ", " << a << ") for '" << objectName << "'." << std::endl;
}