#include "BossObject.h"
#include <cstdlib>
#include <algorithm>
#include "Game.h"

std::vector<Bullet> Boss::bulletBoss;

Boss::Boss()
    : bossTexture(nullptr), bulletTexture(nullptr), active(false), speed(5), health(100),
      bulletSpeed(10), explosionStartTime(0), lastShootTime(0)
{
    rect = {SCREEN_WIDTH - 150, SCREEN_HEIGHT / 2 - 50, 150, 200};
}

Boss::~Boss()
{
    if (bossTexture) SDL_DestroyTexture(bossTexture);
    if (bulletTexture) SDL_DestroyTexture(bulletTexture);
}

void Boss::loadBossImage(SDL_Renderer *renderer, const char *filePath)
{
    if (bossTexture) SDL_DestroyTexture(bossTexture);
    SDL_Surface *surface = IMG_Load(filePath);
    if (surface)
    {
        bossTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else
    {
        std::cout << "Failed to load Boss texture: " << filePath << std::endl;
    }
}

void Boss::loadBulletImage(SDL_Renderer *renderer, const char *filePath)
{
    if (bulletTexture) SDL_DestroyTexture(bulletTexture);
    SDL_Surface *surface = IMG_Load(filePath);
    if (surface)
    {
        bulletTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else
    {
        std::cout << "Failed to load Boss bullet texture: " << filePath << std::endl;
    }
}

void Boss::render(SDL_Renderer *renderer)
{
    if (!active || !bossTexture) return;

    SDL_RenderCopy(renderer, bossTexture, nullptr, &rect);

    // Draw health bar (smooth)
    float healthPercent = std::max(0.0f, std::min(1.0f, health / 100.0f));
    int healthBarWidth = static_cast<int>(rect.w * healthPercent);
    SDL_Rect healthBar = {rect.x, rect.y - 15, healthBarWidth, 8};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthBar);

    // Draw boss bullets
    for (const auto &bullet : bulletBoss)
    {
        if (bullet.active && bulletTexture)
        {
            SDL_RenderCopy(renderer, bulletTexture, nullptr, &bullet.rect);
        }
    }

    // Draw explosion if needed
    if (exp1.isExploding())
    {
        exp1.render(renderer);
    }
}

void Boss::update()
{
    if (!active) return;

    // Move boss vertically
    rect.y += speed;
    if (rect.y <= 0 || rect.y >= SCREEN_HEIGHT - rect.h)
        speed = -speed;

    // Update boss bullets
    for (auto &bullet : bulletBoss)
    {
        if (bullet.active)
        {
            bullet.rect.x -= bulletSpeed;
            if (bullet.rect.x + bullet.rect.w < 0)
                bullet.active = false;
        }
    }
    // Remove inactive bullets
    bulletBoss.erase(
        std::remove_if(bulletBoss.begin(), bulletBoss.end(),
                       [](const Bullet &b) { return !b.active; }),
        bulletBoss.end());

    // Shoot bullet every 500ms
    Uint32 now = SDL_GetTicks();
    if (now - lastShootTime > 500)
    {
        shootBullet();
        lastShootTime = now;
    }

    // Update explosion
    if (exp1.isExploding())
        exp1.update();
}

// ...existing code...

bool Boss::handleBulletCollision()
{
    for (auto &bullet : Game::bullets)
    {
        if (bullet.active && SDL_HasIntersection(&bullet.rect, &rect))
        {
            bullet.active = false;
            health -= 4;
            if (health <= 0)
            {
                health = 0;
                active = false;
                // Không gọi startExplosion() ở đây, hãy gọi ở ngoài và truyền renderer vào
                return true;
            }
        }
    }
    return false;
}

void Boss::startExplosion(SDL_Renderer* renderer)
{
    if (!exp1.isExploding())
    {
        exp1.loadExplosionImage(renderer, "image/Exp_main.png");
        exp1.start(rect.x, rect.y);
        explosionStartTime = SDL_GetTicks();
    }
}

void Boss::loadExplosionBoss(SDL_Renderer *renderer)
{
    if (health <= 0)
        startExplosion(renderer);
    if (exp1.isExploding())
        exp1.update();
}

void Boss::shootBullet()
{
    SDL_Rect bulletRect = {rect.x - 30, rect.y + rect.h / 2 - 25, 50, 50};
    Bullet newBullet(bulletRect, true);
    bulletBoss.push_back(newBullet);
}