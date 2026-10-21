#include "Achievement.h"
#include <iostream>

AchievementSystem::AchievementSystem()
    : recentUnlock(ACH_FIRST_BLOOD), notificationTime(0), showingNotification(false)
{
}

AchievementSystem::~AchievementSystem()
{
}

void AchievementSystem::initialize()
{
    achievements.clear();
    
    achievements.push_back(Achievement(ACH_FIRST_BLOOD, "First Blood", 
                                      "Destroy your first enemy", 1));
    achievements.push_back(Achievement(ACH_BOSS_SLAYER, "Boss Slayer", 
                                      "Defeat the boss", 1));
    achievements.push_back(Achievement(ACH_UNTOUCHABLE, "Untouchable", 
                                      "Complete a level without taking damage", 1));
    achievements.push_back(Achievement(ACH_MARKSMAN, "Marksman", 
                                      "100% accuracy in a level", 1));
    achievements.push_back(Achievement(ACH_COLLECTOR, "Collector", 
                                      "Collect 10 power-ups", 10));
    achievements.push_back(Achievement(ACH_SURVIVOR, "Survivor", 
                                      "Survive for 5 minutes", 1));
    achievements.push_back(Achievement(ACH_COMBO_MASTER, "Combo Master", 
                                      "Achieve a x10 combo", 1));
    achievements.push_back(Achievement(ACH_DESTROYER, "Destroyer", 
                                      "Destroy 100 enemies", 100));
    achievements.push_back(Achievement(ACH_SPEED_RUNNER, "Speed Runner", 
                                      "Complete a level under 2 minutes", 1));
    achievements.push_back(Achievement(ACH_MASTER, "Master", 
                                      "Unlock all achievements", 9));
}

void AchievementSystem::updateProgress(AchievementID id, int value)
{
    for (auto& ach : achievements)
    {
        if (ach.id == id && !ach.unlocked)
        {
            ach.progress += value;
            if (ach.progress >= ach.target)
            {
                unlock(id);
            }
            break;
        }
    }
}

void AchievementSystem::unlock(AchievementID id)
{
    for (auto& ach : achievements)
    {
        if (ach.id == id && !ach.unlocked)
        {
            ach.unlocked = true;
            ach.progress = ach.target;
            std::cout << "Achievement Unlocked: " << ach.name << std::endl;
            
            // Show notification
            recentUnlock = id;
            notificationTime = SDL_GetTicks();
            showingNotification = true;
            
            // Check if all achievements are unlocked
            if (getUnlockedCount() >= 9)
            {
                updateProgress(ACH_MASTER, 1);
            }
            break;
        }
    }
}

bool AchievementSystem::isUnlocked(AchievementID id) const
{
    for (const auto& ach : achievements)
    {
        if (ach.id == id)
            return ach.unlocked;
    }
    return false;
}

int AchievementSystem::getUnlockedCount() const
{
    int count = 0;
    for (const auto& ach : achievements)
    {
        if (ach.unlocked && ach.id != ACH_MASTER)
            count++;
    }
    return count;
}

void AchievementSystem::render(SDL_Renderer* renderer, TTF_Font* font)
{
    if (!showingNotification)
        return;
    
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - notificationTime > 3000) // Show for 3 seconds
    {
        showingNotification = false;
        return;
    }
    
    showNotification(renderer, font, recentUnlock);
}

void AchievementSystem::showNotification(SDL_Renderer* renderer, TTF_Font* font, AchievementID id)
{
    if (!font)
        return;
    
    for (const auto& ach : achievements)
    {
        if (ach.id == id)
        {
            // Draw notification box
            SDL_Rect box = {50, 50, 400, 100};
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_RenderFillRect(renderer, &box);
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            SDL_RenderDrawRect(renderer, &box);
            
            // Draw text
            SDL_Color color = {255, 215, 0, 255};
            SDL_Surface* surface = TTF_RenderUTF8_Blended(font, "Achievement Unlocked!", color);
            if (surface)
            {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int tw, th;
                SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
                SDL_Rect textRect = {box.x + 10, box.y + 10, tw, th};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
            
            SDL_Color nameColor = {255, 255, 255, 255};
            surface = TTF_RenderUTF8_Blended(font, ach.name.c_str(), nameColor);
            if (surface)
            {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                int tw, th;
                SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
                SDL_Rect textRect = {box.x + 10, box.y + 40, tw, th};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
            
            break;
        }
    }
}
