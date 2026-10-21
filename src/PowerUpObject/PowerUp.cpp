#include "PowerUp.h"
#include <cstdlib>

PowerUp::PowerUp()
    : texture(nullptr), active(false), speed(3), duration(5000)
{
    rect = {0, 0, 40, 40};
    type = POWERUP_HEALTH;
}

PowerUp::PowerUp(int x, int y, PowerUpType pType)
    : texture(nullptr), active(true), speed(3), type(pType)
{
    rect = {x, y, 40, 40};
    
    // Set duration based on type
    switch(type)
    {
        case POWERUP_SHIELD:
            duration = 8000;
            break;
        case POWERUP_RAPID_FIRE:
            duration = 10000;
            break;
        case POWERUP_DOUBLE_SHOT:
            duration = 12000;
            break;
        case POWERUP_HEALTH:
            duration = 0; // Instant effect
            break;
        case POWERUP_BOMB:
            duration = 0; // Instant effect
            break;
    }
}

PowerUp::~PowerUp()
{
    if (texture)
        SDL_DestroyTexture(texture);
}

void PowerUp::update()
{
    if (!active)
        return;
    
    // Move left
    rect.x -= speed;
    
    // Deactivate if out of screen
    if (rect.x + rect.w < 0)
        active = false;
}

void PowerUp::render(SDL_Renderer* renderer)
{
    if (!active || !texture)
        return;
    
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void PowerUp::loadTexture(SDL_Renderer* renderer, const char* filePath)
{
    if (texture)
        SDL_DestroyTexture(texture);
    
    SDL_Surface* surface = IMG_Load(filePath);
    if (surface)
    {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}
