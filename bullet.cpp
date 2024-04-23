#include "game.h"
#include "bossobject.h"

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
  Bullet bullet;
  bullet.rect.x = gSquareRect.x + gSquareRect.w / 2;
  bullet.rect.y = gSquareRect.y + gSquareRect.h / 2 - 25;
  bullet.rect.w = 50;
  bullet.rect.h = 50;
  bullet.active = true;
  bullets.push_back(bullet);
}

void Game::updateBullets()
{
  for (auto &bullet : bullets)
  {
    if (bullet.active)
    {
      bullet.rect.x += 5; // Bullet speed
      if (bullet.rect.x > SCREEN_WIDTH)
      {
        bullet.active = false;
      }
    }
  }
}

void Game::handleBossBulletCollision()
{
  for (auto &bulletB : Boss::getBulletBoss())
  {
    if (SDL_HasIntersection(&gSquareRect, &bulletB.rect)) // Xử lý khi va chạm đạn của boss và nhân vật
    {
      currentHealth -= 1;
      bulletB.active = false;
      bulletB.rect = {0, 0, 0, 0};
      if (Mix_PlayChannel(-1, gCollision, 0) == -1) // SoundVaCham
      {
        std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
      }
    }
  }
}