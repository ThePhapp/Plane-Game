#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <string>
#include <fstream>
#include <iostream>

struct GameData
{
    int highScore;
    int coins;
    int level;
    int weaponLevel;
    bool achievements[10]; // 10 achievements
};

class SaveGame
{
public:
    SaveGame();
    ~SaveGame();
    
    bool saveProgress(const std::string& filename, const GameData& data);
    bool loadProgress(const std::string& filename, GameData& data);
    
    GameData& getData() { return gameData; }
    void setData(const GameData& data) { gameData = data; }
    
private:
    GameData gameData;
    const std::string SAVE_DIR = "save/";
};

#endif // SAVEGAME_H
