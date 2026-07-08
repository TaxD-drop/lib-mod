#include "modmenu.h"
#include "imgui.h"

ModMenu& ModMenu::get() {
    static ModMenu instance;
    return instance;
}

void ModMenu::initialize(void* window) {
    // Inicialização já feita em main.cpp
}

void ModMenu::render() {
    if (!visible) return;

    ImGui::Begin("Mod Menu", &visible, ImGuiWindowFlags_AlwaysAutoResize);
    for (auto& item : items) {
        if (ImGui::Button(item.label.c_str())) {
            item.callback();
        }
    }
    ImGui::End();
}

void ModMenu::addItem(const std::string& label, std::function<void()> action) {
    items.push_back({label, action});
}