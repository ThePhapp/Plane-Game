#include "Leaderboard.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <direct.h>

Leaderboard::Leaderboard()
    : maxEntries(10)
{
    _mkdir(SAVE_DIR.c_str());
}

Leaderboard::~Leaderboard()
{
}

void Leaderboard::addScore(const ScoreEntry& entry)
{
    topScores.push_back(entry);
    sortScores();
    
    // Keep only top scores
    if (topScores.size() > maxEntries)
    {
        topScores.resize(maxEntries);
    }
}

void Leaderboard::sortScores()
{
    std::sort(topScores.begin(), topScores.end(), 
              [](const ScoreEntry& a, const ScoreEntry& b) { return a > b; });
}

void Leaderboard::saveToFile(const std::string& filename)
{
    std::string fullPath = SAVE_DIR + filename;
    std::ofstream file(fullPath);
    
    if (!file.is_open())
    {
        std::cout << "Failed to save leaderboard to " << fullPath << std::endl;
        return;
    }
    
    file << topScores.size() << "\n";
    for (const auto& entry : topScores)
    {
        file << entry.playerName << "\n";
        file << entry.score << "\n";
        file << entry.level << "\n";
        file << entry.date << "\n";
    }
    
    file.close();
    std::cout << "Leaderboard saved to " << fullPath << std::endl;
}

void Leaderboard::loadFromFile(const std::string& filename)
{
    std::string fullPath = SAVE_DIR + filename;
    std::ifstream file(fullPath);
    
    if (!file.is_open())
    {
        std::cout << "No leaderboard file found at " << fullPath << std::endl;
        return;
    }
    
    topScores.clear();
    int count;
    file >> count;
    file.ignore(); // Skip newline
    
    for (int i = 0; i < count; i++)
    {
        ScoreEntry entry;
        std::getline(file, entry.playerName);
        file >> entry.score;
        file >> entry.level;
        file >> entry.date;
        file.ignore(); // Skip newline
        
        topScores.push_back(entry);
    }
    
    file.close();
    std::cout << "Leaderboard loaded from " << fullPath << std::endl;
}

void Leaderboard::render(SDL_Renderer* renderer, TTF_Font* font)
{
    if (!font)
        return;
    
    // Draw background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect bg = {100, 100, 1080, 620};
    SDL_RenderFillRect(renderer, &bg);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    SDL_RenderDrawRect(renderer, &bg);
    
    // Draw title
    SDL_Color titleColor = {255, 215, 0, 255};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, "LEADERBOARD", titleColor);
    if (surface)
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int tw, th;
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
        SDL_Rect textRect = {640 - tw/2, 120, tw, th};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    
    // Draw scores
    SDL_Color textColor = {255, 255, 255, 255};
    int yPos = 200;
    int rank = 1;
    
    for (const auto& entry : topScores)
    {
        std::string scoreText = std::to_string(rank) + ". " + entry.playerName + 
                               " - " + std::to_string(entry.score) + " pts (Lvl " + 
                               std::to_string(entry.level) + ")";
        
        surface = TTF_RenderUTF8_Blended(font, scoreText.c_str(), textColor);
        if (surface)
        {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            int tw, th;
            SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
            SDL_Rect textRect = {150, yPos, tw, th};
            SDL_RenderCopy(renderer, texture, NULL, &textRect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
        
        yPos += 50;
        rank++;
    }
}
