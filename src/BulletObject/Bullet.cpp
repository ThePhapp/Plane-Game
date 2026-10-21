#include "Game.h"
#include "BossObject/BossObject.h"
#include <algorithm>

bool Game::fireBullet()
{
    Uint32 currentTime = SDL_GetTicks();
    int fireRate = hasRapidFire ? 100 : playerWeapon.getFireRate();
    
    // Check fire rate limit
    if (lastFireTime != 0 && currentTime - lastFireTime < static_cast<Uint32>(fireRate))
        return false;
    
    lastFireTime = currentTime;
    shotsFired++;

    auto addBullet = [this](int yOffset, int width, int height)
    {
        Bullet bullet;
        bullet.rect = {
            gSquareRect.x + gSquareRect.w / 2,
            gSquareRect.y + gSquareRect.h / 2 - height / 2 + yOffset,
            width,
            height
        };
        bullet.active = true;
        bullets.push_back(bullet);
    };

    switch (playerWeapon.getType())
    {
        case WEAPON_LASER:
            addBullet(0, 70, 14);
            break;
        case WEAPON_MISSILE:
            addBullet(0, 60, 30);
            break;
        case WEAPON_SPREAD:
            addBullet(-24, 50, 50);
            addBullet(0, 50, 50);
            addBullet(24, 50, 50);
            break;
        case WEAPON_NORMAL:
        default:
            addBullet(0, 50, 50);
            break;
    }

    if (hasDoubleShot)
        addBullet(10, 50, 50);

    return true;
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
            bulletB.active = false;
            bulletB.rect = {0, 0, 0, 0};
            damagePlayer();
        }
    }
}
