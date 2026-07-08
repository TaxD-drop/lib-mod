// mods/example_mod.cpp
#include "../include/mod_interface.h"
#include <iostream>

class ExampleMod : public IMod {
public:
    const char* getName() const override { return "ExampleMod"; }
    void onLoad() override {
        std::cout << "[ExampleMod] Carregado!\n";
    }
    void onUpdate(float deltaTime) override {
        std::cout << "[ExampleMod] Update, dt = " << deltaTime << "\n";
    }
    void onUnload() override {
        std::cout << "[ExampleMod] Descarregado!\n";
    }
};

extern "C" IMod* create_mod() {
    return new ExampleMod();
}