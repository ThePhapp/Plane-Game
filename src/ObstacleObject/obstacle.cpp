#include "game.h"

void Game::updateObstacle()
{
    if (!isObstacleActive)
    {
        spawnObstacle();
    }

    int obstacleSpeed = 4;
    if (level == 1)
        obstacleSpeed = 4;
    else if (level == 2)
        obstacleSpeed = 8;
    else if (level == 3)
        obstacleSpeed = 12;

    gObstacleRect.x -= obstacleSpeed;

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