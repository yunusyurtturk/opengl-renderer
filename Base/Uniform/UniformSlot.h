#pragma once
#include <SDL_opengl.h>

class UniformSlot {
public:
    virtual ~UniformSlot() = default;
    virtual void Update() = 0;
};