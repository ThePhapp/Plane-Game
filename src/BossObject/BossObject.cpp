#include "BossObject.h"
#include <cstdlib>
#include <algorithm>
#include "Game.h"

std::vector<Bullet> Boss::bulletBoss;

Boss::Boss()
    : bossTexture(nullptr), bulletTexture(nullptr), active(false), speed(5), health(100),
      maxHealth(100), bulletSpeed(10), type(BOSS_NORMAL), phase(1), 
      explosionStartTime(0), lastShootTime(0)
{
    rect = {SCREEN_WIDTH - 150, SCREEN_HEIGHT / 2 - 50, 150, 200};
}

Boss::Boss(BossType bType)
    : bossTexture(nullptr), bulletTexture(nullptr), active(false), type(bType), 
      phase(1), explosionStartTime(0), lastShootTime(0)
{
    // Set properties based on boss type
    switch(type)
    {
        case BOSS_NORMAL:
            speed = 5;
            health = 100;
            maxHealth = 100;
            bulletSpeed = 10;
            rect = {SCREEN_WIDTH - 150, SCREEN_HEIGHT / 2 - 50, 150, 200};
            break;
            
        case BOSS_FAST:
            speed = 8;
            health = 80;
            maxHealth = 80;
            bulletSpeed = 15;
            rect = {SCREEN_WIDTH - 130, SCREEN_HEIGHT / 2 - 40, 130, 160};
            break;
            
        case BOSS_TANK:
            speed = 3;
            health = 200;
            maxHealth = 200;
            bulletSpeed = 8;
            rect = {SCREEN_WIDTH - 180, SCREEN_HEIGHT / 2 - 60, 180, 240};
            break;
            
        case BOSS_ULTIMATE:
            speed = 6;
            health = 300;
            maxHealth = 300;
            bulletSpeed = 12;
            rect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT / 2 - 70, 200, 280};
            break;
    }
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
    float healthPercent = std::max(0.0f, std::min(1.0f, (float)health / maxHealth));
    int healthBarWidth = static_cast<int>(rect.w * healthPercent);
    SDL_Rect healthBar = {rect.x, rect.y - 15, healthBarWidth, 8};
    
    // Color based on health
    if (healthPercent > 0.5f)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    else if (healthPercent > 0.25f)
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
    else
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        
    SDL_RenderFillRect(renderer, &healthBar);
    
    // Draw health bar border
    SDL_Rect healthBarBorder = {rect.x, rect.y - 15, rect.w, 8};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &healthBarBorder);

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

void Boss::update(float deltaSeconds)
{
    if (exp1.isExploding())
        exp1.update();

    if (!active) return;
    
    // Check phase change
    float healthPercent = (float)health / maxHealth;
    if (healthPercent <= 0.5f && phase == 1)
    {
        phase = 2;
        std::cout << "Boss entered Phase 2!" << std::endl;
        // Increase difficulty in phase 2
        if (type == BOSS_ULTIMATE)
        {
            bulletSpeed = (int)(bulletSpeed * 1.5);
        }
    }

    // Move boss vertically
    rect.y += static_cast<int>(speed * SCREEN_FPS * deltaSeconds);
    if (rect.y <= 0)
    {
        rect.y = 0;
        speed = std::abs(speed);
    }
    else if (rect.y >= SCREEN_HEIGHT - rect.h)
    {
        rect.y = SCREEN_HEIGHT - rect.h;
        speed = -std::abs(speed);
    }

    // Update boss bullets
    for (auto &bullet : bulletBoss)
    {
        if (bullet.active)
        {
            bullet.rect.x -= static_cast<int>(bulletSpeed * SCREEN_FPS * deltaSeconds);
            if (bullet.rect.x + bullet.rect.w < 0)
                bullet.active = false;
        }
    }
    // Remove inactive bullets
    bulletBoss.erase(
        std::remove_if(bulletBoss.begin(), bulletBoss.end(),
                       [](const Bullet &b) { return !b.active; }),
        bulletBoss.end());

    // Shoot based on type and phase
    Uint32 now = SDL_GetTicks();
    int shootInterval = (phase == 2) ? 300 : 500; // Faster in phase 2
    
    if (now - lastShootTime > shootInterval)
    {
        shootPattern();
        lastShootTime = now;
    }

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

void Boss::shootPattern()
{
    switch(type)
    {
        case BOSS_NORMAL:
            // Simple straight shot
            shootBullet();
            break;
            
        case BOSS_FAST:
            // Triple shot
            if (phase == 2)
            {
                // 3 bullets spread
                for (int i = -1; i <= 1; i++)
                {
                    SDL_Rect bulletRect = {rect.x - 30, rect.y + rect.h / 2 + (i * 40), 50, 50};
                    Bullet newBullet(bulletRect, true);
                    bulletBoss.push_back(newBullet);
                }
            }
            else
            {
                shootBullet();
            }
            break;
            
        case BOSS_TANK:
            {
                // Burst fire (2 bullets)
                shootBullet();
                SDL_Rect bulletRect2 = {rect.x - 30, rect.y + rect.h / 2 + 30, 50, 50};
                Bullet newBullet2(bulletRect2, true);
                bulletBoss.push_back(newBullet2);
                break;
            }
            
        case BOSS_ULTIMATE:
            // Complex pattern based on phase
            if (phase == 2)
            {
                // 5-way spread
                for (int i = -2; i <= 2; i++)
                {
                    SDL_Rect bulletRect = {rect.x - 30, rect.y + rect.h / 2 + (i * 30), 50, 50};
                    Bullet newBullet(bulletRect, true);
                    bulletBoss.push_back(newBullet);
                }
            }
            else
            {
                // 3-way spread
                for (int i = -1; i <= 1; i++)
                {
                    SDL_Rect bulletRect = {rect.x - 30, rect.y + rect.h / 2 + (i * 50), 50, 50};
                    Bullet newBullet(bulletRect, true);
                    bulletBoss.push_back(newBullet);
                }
            }
            break;
    }
}
