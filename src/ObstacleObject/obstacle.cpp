#include "game.h"

void Game::updateObstacle()
{
    if (!isObstacleActive)
    {
        spawnObstacle();
    }

    if (0 <= points && points < 200)
    {
        gObstacleRect.x -= 4;
        level = 1;
    }
    else if (200 <= points && points < 1800)
    {
        gObstacleRect.x -= 8;
        level = 2;
    }
    else
    {
        gObstacleRect.x -= 12;
        level = 3;
    }

    if (gObstacleRect.x + gObstacleRect.w < 0)
    {
        isObstacleActive = false;
        miss++;
    }
}

void Game::spawnObstacle()
{
    /// make obstacle
    gObstacleRect = {SCREEN_WIDTH - 50, rand() % (SCREEN_HEIGHT - 50), 80, 80};
    isObstacleActive = true;
}