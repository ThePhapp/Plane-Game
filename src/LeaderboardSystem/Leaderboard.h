#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct ScoreEntry
{
    std::string playerName;
    int score;
    int level;
    time_t date;
    
    ScoreEntry(const std::string& name = "Player", int scr = 0, int lvl = 1)
        : playerName(name), score(scr), level(lvl)
    {
        date = time(nullptr);
    }
    
    bool operator>(const ScoreEntry& other) const
    {
        return score > other.score;
    }
};

class Leaderboard
{
public:
    Leaderboard();
    ~Leaderboard();
    
    void addScore(const ScoreEntry& entry);
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void render(SDL_Renderer* renderer, TTF_Font* font);
    
    std::vector<ScoreEntry>& getTopScores() { return topScores; }
    int getMaxEntries() const { return maxEntries; }

private:
    std::vector<ScoreEntry> topScores;
    int maxEntries;
    const std::string SAVE_DIR = "save/";
    
    void sortScores();
};

#endif // LEADERBOARD_H
