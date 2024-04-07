#include "game.h"

void Game::handleBulletCollision()
{    // Đạn trúng -> dừng
    if (isBulletActive && SDL_HasIntersection(&gBulletRect, &gObstacleRect))
    {
        isBulletActive = false;
        isObstacleActive = false;
        this->points += 100;
        if (Mix_PlayChannel(-1, gShotTrung, 0) == -1)
            {
              std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
            }  
    }
}
