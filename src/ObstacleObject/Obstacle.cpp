#include "Obstacle.h"
#include <cmath>
#include "Config.h"

Obstacle::Obstacle()
    : texture(nullptr), type(ENEMY_BASIC), active(false), speed(4), health(10), points(100), maxHealth(10)
{
    rect = {0, 0, 80, 80};
}

Obstacle::Obstacle(int x, int y, EnemyType eType)
    : texture(nullptr), type(eType), active(true)
{
    rect = {x, y, 80, 80};
    
    // Set properties based on enemy type
    switch(type)
    {
        case ENEMY_BASIC:
            speed = 4;
            health = 10;
            maxHealth = 10;
            points = 100;
            rect.w = rect.h = 80;
            break;
            
        case ENEMY_FAST:
            speed = 8;
            health = 5;
            maxHealth = 5;
            points = 150;
            rect.w = rect.h = 60;
            break;
            
        case ENEMY_TANK:
            speed = 2;
            health = 30;
            maxHealth = 30;
            points = 300;
            rect.w = rect.h = 100;
            break;
            
        case ENEMY_KAMIKAZE:
            speed = 6;
            health = 8;
            maxHealth = 8;
            points = 200;
            rect.w = rect.h = 70;
            break;
    }
}

Obstacle::~Obstacle()
{
}

void Obstacle::update(SDL_Rect playerRect, float deltaSeconds)
{
    if (!active)
        return;
    
    const float movement = speed * SCREEN_FPS * deltaSeconds;

    if (type == ENEMY_KAMIKAZE)
    {
        // Kamikaze moves toward player
        float dx = playerRect.x - rect.x;
        float dy = playerRect.y - rect.y;
        float distance = sqrt(dx*dx + dy*dy);
        
        if (distance > 0)
        {
            rect.x += static_cast<int>((dx / distance) * movement);
            rect.y += static_cast<int>((dy / distance) * movement);
        }
    }
    else
    {
        // Normal movement (left)
        rect.x -= static_cast<int>(movement);
    }
    
    // Deactivate if out of screen
    if (rect.x + rect.w < 0 || rect.x > 1280 || rect.y < -100 || rect.y > 820)
        active = false;
}

void Obstacle::render(SDL_Renderer* renderer)
{
    if (!active || !texture)
        return;
    
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    
    // Draw health bar for tanks
    if (type == ENEMY_TANK && health > 0)
    {
        float healthPercent = (float)health / maxHealth;
        int barWidth = (int)(rect.w * healthPercent);
        SDL_Rect healthBar = {rect.x, rect.y - 10, barWidth, 5};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &healthBar);
    }
}

bool Obstacle::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0)
    {
        active = false;
        return true; // Enemy destroyed
    }
    return false; // Enemy still alive
}

