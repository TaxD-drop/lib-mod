#include "hooking.h"
#include <dobby.h>
#include <android/log.h>

bool Hooking::installHook(void* target, void* replace, void** orig) {
    auto result = DobbyHook(target, replace, orig);
    if (result != 0) {
        __android_log_print(ANDROID_LOG_ERROR, "libmod", "Falha ao hookar");
        return false;
    }
    return true;
}

bool Hooking::hookFunction(const std::string& libName, const std::string& funcName,
                           void* replace, void** orig) {
    // Para simplificar, usando dlsym (requer lib carregada)
    void* handle = dlopen(libName.c_str(), RTLD_NOW);
    if (!handle) return false;
    void* target = dlsym(handle, funcName.c_str());
    dlclose(handle);
    if (!target) return false;
    return installHook(target, replace, orig);
}