#include "SaveGame.h"
#include <filesystem>

SaveGame::SaveGame()
{
    gameData = {};
    std::filesystem::create_directories(SAVE_DIR);
}

SaveGame::~SaveGame()
{
}

bool SaveGame::saveProgress(const std::string& filename, const GameData& data)
{
    std::string fullPath = SAVE_DIR + filename;
    std::ofstream file(fullPath, std::ios::binary);
    
    if (!file.is_open())
    {
        std::cout << "Failed to save game to " << fullPath << std::endl;
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(&data), sizeof(GameData));
    if (!file)
    {
        std::cout << "Failed to write save data to " << fullPath << std::endl;
        return false;
    }
    
    std::cout << "Game saved successfully to " << fullPath << std::endl;
    return true;
}

bool SaveGame::loadProgress(const std::string& filename, GameData& data)
{
    std::string fullPath = SAVE_DIR + filename;
    std::ifstream file(fullPath, std::ios::binary);
    
    if (!file.is_open())
    {
        std::cout << "No save file found at " << fullPath << std::endl;
        // Return default data
        data = {};
        return false;
    }
    
    file.read(reinterpret_cast<char*>(&data), sizeof(GameData));
    if (!file || file.gcount() != static_cast<std::streamsize>(sizeof(GameData)))
    {
        data = {};
        std::cout << "Save file is invalid or incomplete: " << fullPath << std::endl;
        return false;
    }

    if (data.highScore < 0 || data.coins < 0 ||
        data.level < 1 || data.level > 3 ||
        data.weaponLevel < 1 || data.weaponLevel > 5)
    {
        data = {};
        std::cout << "Save file contains invalid progression values: " << fullPath << std::endl;
        return false;
    }
    
    std::cout << "Game loaded successfully from " << fullPath << std::endl;
    return true;
}
