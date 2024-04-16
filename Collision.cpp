
#include "collision.h"
//Return false: hitbox err
bool Collision::checkCollision(SDL_Rect a, SDL_Rect b)
{
    if (a.x + a.w >= b.x && a.x <= b.x + b.w &&
        a.y + a.h >= b.y && a.y <= b.y + b.h)
    {
        return true;
    }
    return false;
}
