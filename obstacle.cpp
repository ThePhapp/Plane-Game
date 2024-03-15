#include "game.h"

void Game::updateObstacle()
{
    if (!isObstacleActive)
    {
        spawnObstacle();
    }
    // Di chuyển vật vản
    gObstacleRect.x -= 1;

if (gObstacleRect.x + gObstacleRect.w < 0)
{
    isObstacleActive = false;
}
}

void Game::spawnObstacle()
{
    // Sinh vật cản
    gObstacleRect = {SCREEN_WIDTH - 50, rand() % (SCREEN_HEIGHT - 50), 50, 50};
    isObstacleActive = true;
}