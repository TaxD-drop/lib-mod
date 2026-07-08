#pragma once
#include <string>
#include <vector>
#include <functional>

struct MenuItem {
    std::string label;
    std::function<void()> callback;
};

class ModMenu {
public:
    static ModMenu& get();
    void initialize(void* window);  // para Android, window é a ANativeWindow*
    void render();
    void addItem(const std::string& label, std::function<void()> action);
    bool isVisible() const { return visible; }
    void toggle() { visible = !visible; }

private:
    ModMenu() = default;
    bool visible = true;
    std::vector<MenuItem> items;
};