#pragma once

template <typename T>
T GetOffsetValue(void* _this, unsigned int offset, const char* library, const char* className)
{
	GET_CLASS(library, className, pClass) {
		try {
			return pClass->GetValue<T>(static_cast<void*>(_this), offset);
		}
		catch (const std::exception& ex) {
			//LOGD(std::format("Error retrieving: {}", ex.what()));
		}
	}
	return T{};
}

template <typename HookFunc>
void setupHook(const char* dllName, const char* className, const char* methodName, HookFunc hookFunction) {
	GET_CLASS(dllName, className, pClass) {
		try {
			auto methodPointer = pClass->Get<UnityResolve::Method>(methodName)->Cast<HookFunc>();
			if (methodPointer) {
				//LOGD(std::format("Lib: {} | Class: {} | Method: {} -> success hooked!", dllName, className, methodName));
				HookManager::InstallPtr(reinterpret_cast<void*>(methodPointer), reinterpret_cast<void*>(hookFunction));
			}
			else {
				//LOGD(std::format("Error: method {} not found in class {}", methodName, className));
			}
		}
		catch (const std::exception& ex) {
			//LOGD(std::format("Error setting up hook for {}::{}: {}", className, methodName, ex.what()));
		}
	}
}

template <typename HookFunc>
void setupRVAHook(void* funcAddress, HookFunc hookFunction) {
	try {
		auto methodPointer = UnityResolve::UnityType::AddrM(funcAddress);
		if (methodPointer) {
			//LOGD("Method address resolved -> success hooked!");
			HookManager::InstallPtr(methodPointer, reinterpret_cast<void*>(hookFunction));
		}
		else {
			//LOGD("Error: method address could not be resolved.");
		}
	}
	catch (const std::exception& ex) {
		//LOGD(std::format("Error setting up hook: {}", ex.what()));
	}
}

template<typename T>
void logOffsetValue(void* _this, unsigned int offset, const char* library, const char* className, const char* offsetName) {
	T value = GetOffsetValue<T>(_this, offset, library, className);

	if constexpr (std::is_same<T, bool>::value) {
		//LOGD(std::format("{} 0x{:X}: {}", offsetName, offset, value ? "true" : "false"));
	}
	else if constexpr (std::is_integral<T>::value) {
		//LOGD(std::format("{} 0x{:X}: {}", offsetName, offset, value));
	}
	else if constexpr (std::is_floating_point<T>::value) {
		//LOGD(std::format("{} 0x{:X}: {:.2f}", offsetName, offset, value));
	}
	else if constexpr (std::is_same<T, std::string>::value) {
		//LOGD(std::format("{} 0x{:X}: {}", offsetName, offset, value));
	}
	else {
		//LOGD(std::format("{} 0x{:X}: Unknown type", offsetName, offset));
	}
}

#define INVOKE_METHOD_EA(result_type, assembly, class_name, method_name)    \
    ([]() -> result_type {                                              \
        static I::Method* method = nullptr;                             \
        if (!method) {                                                  \
            method = UnityResolve::Get(assembly)->Get(class_name)->Get<UnityResolve::Method>(method_name); \
        }                                                               \
        return method ? method->Invoke<result_type>() : result_type();   \
    })()