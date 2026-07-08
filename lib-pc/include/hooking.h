#pragma once
#include <cstdint>
#include <string>
#include <functional>

namespace Hooking {
    // Função genérica para instalar hook (usará Dobby internamente)
    bool installHook(void* target, void* replace, void** orig);
    // Hook para funções por nome (símbolo da lib)
    bool hookFunction(const std::string& libName, const std::string& funcName,
                      void* replace, void** orig);
}