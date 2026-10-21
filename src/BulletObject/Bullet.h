#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

struct Bullet
{
    SDL_Rect rect;
    bool active;
    Bullet() : rect({0, 0, 0, 0}), active(false) {}
    Bullet(SDL_Rect r, bool a) : rect(r), active(a) {}
};

#endif