#include "../main.h"

std::filesystem::path storagePath;
std::filesystem::path getStorage() {
    if (storagePath.empty()) {
        if (const char* homeDir = std::getenv("USERPROFILE")) {
            storagePath = std::filesystem::path(homeDir) / "PhasmoHook";
        }
    }
    return storagePath;
}