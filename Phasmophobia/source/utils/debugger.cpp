#include "../main.h"

void DebugClass(const char* dll, const char* className) {
    UnityResolve::Class* cls = UnityResolve::Get(dll)->Get(className);
    if (cls) {
        LOGD(std::format("Fields in {}:", className));
        for (const auto* field : cls->fields) {
            LOGD(std::format("Field: {}, Offset: 0x{:x}", field->name, field->offset));
        }
        LOGD(std::format("Methods in {}:", className));
        for (const auto* method : cls->methods) {
            LOGD(std::format("Method: {}", method->name));
        }
    }
    else {
        LOGD(std::format("Failed to find {}", className));
    }
}