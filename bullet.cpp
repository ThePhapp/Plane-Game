#include "game.h"

void Game::handleBulletCollision()
{    // Đạn trúng -> dừng
    if (isBulletActive && SDL_HasIntersection(&gBulletRect, &gObstacleRect))
    {
        isBulletActive = false;
        isObstacleActive = false;
        this->points++;
    }
}
