// src/modlib.h
#pragma once

#include "mod_interface.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

class ModHandle {
public:
    ModHandle(const std::string& path);
    ~ModHandle();

    ModHandle(const ModHandle&) = delete;
    ModHandle& operator=(const ModHandle&) = delete;

    IMod* createMod() const;
    bool isValid() const { return handle != nullptr; }

private:
#ifdef _WIN32
    HMODULE handle;
#else
    void* handle;
#endif
};

class ModManager {
public:
    void loadModsFromDirectory(const std::string& directory);
    void update(float deltaTime);
    void unloadAll();

private:
    struct LoadedMod {
        std::unique_ptr<ModHandle> handle;
        std::unique_ptr<IMod, std::function<void(IMod*)>> instance;
    };
    std::vector<LoadedMod> mods;
};