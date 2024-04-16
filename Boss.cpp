#include "boss.h"
#include <cstdlib>
#include "game.h"

std::vector<Bullet> Boss::bulletBoss;

Boss::Boss()
{
    active = false;
    speed = 1;
    bulletSpeed = 2;
    rect = {SCREEN_WIDTH - 150, SCREEN_HEIGHT / 2 - 50, 150, 200};
    bossTexture = nullptr;
    bulletTexture = nullptr;
    health = 100;
}

Boss::~Boss()
{
    SDL_DestroyTexture(bossTexture);
    SDL_DestroyTexture(bulletTexture);
}

void Boss::render(SDL_Renderer *renderer)
{
    if (active && bossTexture && bulletTexture)
    {
        SDL_RenderCopy(renderer, bossTexture, nullptr, &rect);
        // Render boss health
        int healthBarWidth = static_cast<int>(rect.w * (health / 10) / 10);
        SDL_Rect healthBar = {rect.x, rect.y - 20, healthBarWidth, 10};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &healthBar);

        for (auto &bullet : bulletBoss)
        {
            if (bullet.active)
            {
                SDL_RenderCopy(renderer, bulletTexture, nullptr, &bullet.rect);
            }
        }
    }
}

void Boss::update()
{
    if (active)
    {
        // Set boss speed
        rect.y += speed;
        if (rect.y <= 0 || rect.y >= SCREEN_HEIGHT - rect.h)
        {
            speed = -speed;
        }

        // Update bullet positions and handle collisions
        for (auto &bullet : bulletBoss)
        {
            if (bullet.active)
            {
                bullet.rect.x -= bulletSpeed;
            }
        }

        // Randomly shoot bullets
        if (rand() % 300 == 0)
        {
            shootBullet();
        }
    }
}

void Boss::handleBulletCollision()
{
    for (auto &bullet : Game::bullets)
    {
        // Check collision with boss...
        if (bullet.active && SDL_HasIntersection(&bullet.rect, &rect))
        {
            bullet.active = false;
            health -= 4;
            if (health <= 0)
            {
                active = false;
            }
        }
    }
}

void Boss::shootBullet()
{
    // Adjust bullet position
    SDL_Rect bulletRect = {rect.x - rect.w / 2 + 25, rect.y + rect.h / 2 - 25, 70, 15};
    Bullet newBullet(bulletRect, true);
    bulletBoss.push_back(newBullet);
}

void Boss::loadBossImage(SDL_Renderer *renderer, const char *filePath)
{
    SDL_Surface *surface = IMG_Load(filePath);
    if (surface)
    {
        bossTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else
    {
        std::cout << "Failed to load Boss texture!" << std::endl;
    }
}

void Boss::loadBulletImage(SDL_Renderer *renderer, const char *filePath)
{
    SDL_Surface *surface = IMG_Load(filePath);
    if (surface)
    {
        bulletTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else
    {
        std::cout << "Failed to load Bullet's Boss texture!" << std::endl;
    }
}
