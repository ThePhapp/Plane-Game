
#include "Collision.h"

bool Collision::checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    if (a.x + a.w >= b.x && a.x <= b.x + b.w &&
        a.y + a.h >= b.y && a.y <= b.y + b.h)
    {
        return true;
    }
    return false;
}
