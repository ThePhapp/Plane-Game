#include "game.h"

void Game::updateObstacle()
{
    if (!isObstacleActive)
    {
        spawnObstacle();
    }

    if (0 <= points && points < 300)
    {
        gObstacleRect.x -= 1;
        level = 1;
    }
    else if (300 <= points && points < 1800)
    {
        gObstacleRect.x -= 2;
        level = 2;
    }
    else if (1800 <= points && points < 2500)
    {
        gObstacleRect.x -= 3;
        level = 3;
    }
    else
    {
        gObstacleRect.x -= 4;
        level = 4;
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