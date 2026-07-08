// src/modlib.cpp
#include "modlib.h"
#include <iostream>
#include <filesystem>

#ifdef _WIN32
#define LIB_HANDLE HMODULE
#define LOAD_LIB(path) LoadLibraryA(path.c_str())
#define GET_SYM(handle, name) GetProcAddress(handle, name)
#define CLOSE_LIB(handle) FreeLibrary(handle)
#else
#include <dlfcn.h>
#define LIB_HANDLE void*
#define LOAD_LIB(path) dlopen(path.c_str(), RTLD_NOW)
#define GET_SYM(handle, name) dlsym(handle, name)
#define CLOSE_LIB(handle) dlclose(handle)
#endif

ModHandle::ModHandle(const std::string& path) {
    handle = LOAD_LIB(path);
    if (!handle) {
        std::cerr << "Erro ao carregar " << path << std::endl;
    }
}

ModHandle::~ModHandle() {
    if (handle) {
        CLOSE_LIB(handle);
    }
}

IMod* ModHandle::createMod() const {
    if (!handle) return nullptr;
    auto func = reinterpret_cast<CreateModFunc>(GET_SYM(handle, "create_mod"));
    if (!func) {
        std::cerr << "Símbolo 'create_mod' não encontrado\n";
        return nullptr;
    }
    return func();
}

void ModManager::loadModsFromDirectory(const std::string& directory) {
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            auto path = entry.path().string();
            // Filtra apenas bibliotecas dinâmicas (ajuste conforme SO)
            #ifdef _WIN32
            if (path.find(".dll") == std::string::npos) continue;
            #else
            if (path.find(".so") == std::string::npos) continue;
            #endif

            auto handle = std::make_unique<ModHandle>(path);
            if (!handle->isValid()) continue;

            IMod* rawMod = handle->createMod();
            if (!rawMod) continue;

            rawMod->onLoad();
            mods.push_back({
                std::move(handle),
                std::unique_ptr<IMod, std::function<void(IMod*)>>(
                    rawMod,
                    [](IMod* mod) { mod->onUnload(); delete mod; }
                )
            });
            std::cout << "Mod carregado: " << rawMod->getName() << "\n";
        }
    }
}

void ModManager::update(float deltaTime) {
    for (auto& mod : mods) {
        mod.instance->onUpdate(deltaTime);
    }
}

void ModManager::unloadAll() {
    // A ordem inversa garante que a lib ainda esteja carregada durante onUnload
    while (!mods.empty()) {
        mods.pop_back();  // destrói unique_ptr (chama onUnload, deleta IMod, fecha lib)
    }
}