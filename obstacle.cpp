#include "game.h"

void Game::updateObstacle()
{
    if (!isObstacleActive)
    {
        spawnObstacle();
    }
    gObstacleRect.x -= 2;      //toc do obstacle

if (gObstacleRect.x + gObstacleRect.w < 0)
{
    isObstacleActive = false;
}
}

void Game::spawnObstacle()
{
    ///make obstacle
    gObstacleRect = {SCREEN_WIDTH - 50, rand() % (SCREEN_HEIGHT - 50), 80, 80};
    isObstacleActive = true;
}