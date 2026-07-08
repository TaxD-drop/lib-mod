#pragma once
class IMod {
public:
    virtual ~IMod() = default;
    virtual const char* getName() = 0;
    virtual void onLoad() = 0;
    virtual void onUpdate(float dt) = 0;
    virtual void onUnload() = 0;
};

extern "C" typedef IMod* (*CreateModFunc)();