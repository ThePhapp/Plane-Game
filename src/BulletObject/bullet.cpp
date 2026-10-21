#include "game.h"
#include "BossObject/BossObject.h"
#include <algorithm>

void Game::handleBulletCollision()
{ // Đạn trúng -> dừng
  for (auto &bullet : bullets)
  {
    if (bullet.active && SDL_HasIntersection(&bullet.rect, &gObstacleRect))
    {
      bullet.active = false;
      isObstacleActive = false;
      this->points += 100;
      if (Mix_PlayChannel(-1, gShotTrung, 0) == -1)
      {
        std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
      }
    }
  }
}

void Game::fireBullet()
{
    Uint32 currentTime = SDL_GetTicks();
    int fireRate = hasRapidFire ? 100 : playerWeapon.getFireRate();
    
    // Check fire rate limit
    if (currentTime - lastFireTime < fireRate)
        return;
    
    lastFireTime = currentTime;
    shotsFired++;
    
    // Normal bullet
    Bullet bullet;
    bullet.rect.x = gSquareRect.x + gSquareRect.w / 2;
    bullet.rect.y = gSquareRect.y + gSquareRect.h / 2 - 25;
    bullet.rect.w = 50;
    bullet.rect.h = 50;
    bullet.active = true;
    bullets.push_back(bullet);
    
    // Double shot power-up
    if (hasDoubleShot)
    {
        Bullet bullet2;
        bullet2.rect.x = gSquareRect.x + gSquareRect.w / 2;
        bullet2.rect.y = gSquareRect.y + gSquareRect.h / 2 + 5;
        bullet2.rect.w = 50;
        bullet2.rect.h = 50;
        bullet2.active = true;
        bullets.push_back(bullet2);
    }
}

void Game::updateBullets(float deltaSeconds)
{
    for (auto &bullet : bullets)
    {
        if (bullet.active)
        {
            bullet.rect.x += static_cast<int>(14 * SCREEN_FPS * deltaSeconds); // Bullet speed
            if (bullet.rect.x > SCREEN_WIDTH)
            {
                bullet.active = false;
            }
        }
    }
    
    // Remove inactive bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
                       [](const Bullet &b) { return !b.active; }),
        bullets.end());
}

void Game::renderBullets()
{
    for (const auto &bullet : bullets)
    {
        if (bullet.active)
        {
            SDL_RenderCopy(gRenderer, gBulletTexture, nullptr, &bullet.rect);
        }
    }
}

void Game::handleBossBulletCollision()
{
    for (auto &bulletB : Boss::getBulletBoss())
    {
        if (bulletB.active && SDL_HasIntersection(&gSquareRect, &bulletB.rect))
        {
            if (!hasShield)
            {
                currentHealth -= 1;
                bulletB.active = false;
                bulletB.rect = {0, 0, 0, 0};
                
                if (Mix_PlayChannel(-1, gCollision, 0) == -1)
                {
                    std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }
            }
            else
            {
                // Shield blocks damage
                bulletB.active = false;
                bulletB.rect = {0, 0, 0, 0};
                std::cout << "Shield blocked boss bullet!" << std::endl;
            }
        }
    }
}
