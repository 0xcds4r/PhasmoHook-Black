#include "../main.h"

// universal
bool SetGamma(float gamma) {
    HDC hDC = GetDC(NULL);
    if (!hDC) {
        std::cerr << "Failed to get device context" << std::endl;
        return false;
    }

    WORD gammaRamp[3][256];

    for (int i = 0; i < 256; i++) {
        float normalized = i / 255.0f;
        float adjusted = powf(normalized, 1.0f / gamma);
        WORD value = static_cast<WORD>(adjusted * 65535.0f);

        if (value > 65535) value = 65535;
        if (value < 0) value = 0;

        gammaRamp[0][i] = value;
        gammaRamp[1][i] = value;
        gammaRamp[2][i] = value;
    }

    BOOL result = SetDeviceGammaRamp(hDC, gammaRamp);
    if (!result) {
        std::cerr << "Failed to set gamma ramp" << std::endl;
        ReleaseDC(NULL, hDC);
        return false;
    }

    ReleaseDC(NULL, hDC);
    return true;
}

void ResetGamma() {
    HDC hDC = GetDC(NULL);
    if (!hDC) {
        std::cerr << "Failed to get device context for reset" << std::endl;
        return;
    }

    WORD gammaRamp[3][256];
    for (int i = 0; i < 256; i++) {
        WORD value = i * 257;
        gammaRamp[0][i] = value;
        gammaRamp[1][i] = value;
        gammaRamp[2][i] = value;
    }

    if (!SetDeviceGammaRamp(hDC, gammaRamp)) {
        std::cerr << "Failed to reset gamma ramp" << std::endl;
    }

    ReleaseDC(NULL, hDC);
}

std::string cp1251_to_utf8(const char* cp1251_str) {
    if (!cp1251_str) {
        return "";
    }

    std::string utf8_str;
    const unsigned char* input = reinterpret_cast<const unsigned char*>(cp1251_str);

    for (; *input; ++input) {
        if (*input < 128) {
            utf8_str += static_cast<char>(*input);
        }
        else {
            unsigned char byte = *input;
            unsigned short unicode;

            if (byte >= 0xC0 && byte <= 0xFF) {
                unicode = 0x0410 + (byte - 0xC0);
            }
            else if (byte >= 0xA0 && byte <= 0xBF) {
                unicode = 0x0430 + (byte - 0xA0);
            }
            else if (byte == 0xA8) {
                unicode = 0x0401; 
            }
            else if (byte == 0xB8) {
                unicode = 0x0451; 
            }
            else if (byte == 0xB2) {
                unicode = 0x0402; 
            }
            else if (byte == 0xB3) {
                unicode = 0x0452; 
            }
            else {
                utf8_str += "\xEF\xBF\xBD";
                continue;
            }

            utf8_str += static_cast<char>(0xC0 | ((unicode >> 6) & 0x1F));
            utf8_str += static_cast<char>(0x80 | (unicode & 0x3F));
        }
    }

    return utf8_str;
}

//void printLightNames() 
//{
//    auto* mainCamera = II::Camera::GetMain();
//    if (mainCamera) {
//        auto lights = II::Light::FindAll();
//        if (!lights.empty()) {
//            for (auto* light : lights) {
//                if (light) {
//                    auto* obj = light->GetGameObject();
//                    if (obj) {
//                        std::string stringObjName = std::format("{}", obj->GetName()->ToString());
//                        phasmo.Log(stringObjName);
//                    }
//                }
//            }
//        }
//    }
//}
//
//void printAllColliderNames() {
//    auto* mainCamera = II::Camera::GetMain();
//    if (!mainCamera) return;
//    auto colliders = UnityResolve::UnityType::Collider::FindAll();
//    if (!colliders.empty()) {
//        std::cout << "Colliders found in the scene:" << std::endl;
//        for (auto* collider : colliders) {
//            if (collider) {
//                auto* go = collider->GetGameObject();
//                if (go) {
//                    std::string stringObjName = std::format("{}", go->GetName()->ToString());
//                    std::cout << " - " << stringObjName << std::endl;
//                }
//                else {
//                    std::cout << " - (No GameObject attached)" << std::endl;
//                }
//            }
//        }
//    }
//    else {
//        //std::cerr << "No colliders found in the scene" << std::endl;
//    }
//}