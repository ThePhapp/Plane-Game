#include "SaveGame.h"
#include <direct.h>

SaveGame::SaveGame()
{
    gameData = {0, 0, 1, 1, {false}};
    // Create save directory if not exists
    _mkdir(SAVE_DIR.c_str());
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
    file.close();
    
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
        data = {0, 0, 1, 1, {false}};
        return false;
    }
    
    file.read(reinterpret_cast<char*>(&data), sizeof(GameData));
    file.close();
    
    std::cout << "Game loaded successfully from " << fullPath << std::endl;
    return true;
}
