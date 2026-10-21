#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum AchievementID
{
    ACH_FIRST_BLOOD,      // Tiêu diệt enemy đầu tiên
    ACH_BOSS_SLAYER,      // Đánh bại boss
    ACH_UNTOUCHABLE,      // Hoàn thành level không bị hit
    ACH_MARKSMAN,         // 100% accuracy trong 1 level
    ACH_COLLECTOR,        // Thu thập 10 power-ups
    ACH_SURVIVOR,         // Sống sót 5 phút
    ACH_COMBO_MASTER,     // Combo x10
    ACH_DESTROYER,        // Tiêu diệt 100 enemies
    ACH_SPEED_RUNNER,     // Hoàn thành level dưới 2 phút
    ACH_MASTER            // Mở khóa tất cả achievements
};

struct Achievement
{
    AchievementID id;
    std::string name;
    std::string description;
    bool unlocked;
    int progress;
    int target;
    
    Achievement(AchievementID achId, const std::string& achName, 
                const std::string& desc, int targetValue = 1)
        : id(achId), name(achName), description(desc), 
          unlocked(false), progress(0), target(targetValue) {}
};

class AchievementSystem
{
public:
    AchievementSystem();
    ~AchievementSystem();
    
    void initialize();
    void updateProgress(AchievementID id, int value = 1);
    void unlock(AchievementID id);
    bool isUnlocked(AchievementID id) const;
    void render(SDL_Renderer* renderer, TTF_Font* font);
    void showNotification(SDL_Renderer* renderer, TTF_Font* font, AchievementID id);
    
    std::vector<Achievement>& getAchievements() { return achievements; }
    int getUnlockedCount() const;

private:
    std::vector<Achievement> achievements;
    AchievementID recentUnlock;
    Uint32 notificationTime;
    bool showingNotification;
};

#endif // ACHIEVEMENT_H
