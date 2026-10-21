#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "Config.h"
#include <vector>
#include <array>
#include "BulletObject/Bullet.h"
#include "BossObject/BossObject.h"
#include "ExplosionObject/ExplosionObject.h"
#include "PowerUpObject/PowerUp.h"
#include "WeaponObject/Weapon.h"
#include "SaveSystem/SaveGame.h"
#include "AchievementSystem/Achievement.h"
#include "LeaderboardSystem/Leaderboard.h"
#include "ObstacleObject/Obstacle.h"

class Game
{
public:
    Game();
    ~Game();
    void run();

    static std::vector<Bullet> bullets;

private:
    bool initSDL();
    void closeSDL();
    void render(float deltaSeconds);
    void loadImagee();
    void loadBackground();
    void renderBackground(float deltaSeconds);
    void loadSoundd();
    void resetRun();
    void damagePlayer();
    void renderPoints();
    void renderGameOver();
    void renderWinMenu();
    void renderHealth();
    void handleBossBulletCollision();
    bool fireBullet();
    void updateBullets(float deltaSeconds);
    void handleEvent(SDL_Event &e, bool &quit, float deltaSeconds);
    void loadTextTexture(const std::string &text);
    void loadMenu();
    void handleMainMenuEvent(SDL_Event &e, bool &quit, bool &showMainMenu);
    void handleGameOverEvent(SDL_Event &e, bool &quit, bool &gameOver);
    void renderMainMenu();
    void loadExplosion();
    bool showLevelMenu(bool &quit);
    void applyLevelSettings();
    void renderBullets();
    
    // Power-up functions
    void spawnPowerUp();
    void updatePowerUps(float deltaSeconds);
    void handlePowerUpCollision();
    void applyPowerUpEffect(PowerUpType type);
    void renderPowerUps();
    
    // Obstacle functions
    void spawnObstacleNew();
    void updateObstacles(float deltaSeconds);
    void handleObstacleCollision();
    void renderObstacles();
    
    // System functions
    void saveGame();
    void loadGame();
    void showLeaderboard();
    void showAchievements();

private:
    int backgroundPosX;
    int points = 0;
    int level = 1;
    int highestPoint = 0;
    int maxHealth = 3;
    int currentHealth = 0;
    bool showMainMenu = true;
    bool mouseOverPlay = false;
    bool mouseOverQuit = false;
    const int HEALTH_WIDTH = 50;
    const int HEALTH_HEIGHT = 50;
    const int HEALTH_SPACING = 10;

    Boss boss;
    Explosion exp;
    
    // New systems
    std::vector<PowerUp> powerUps;
    std::vector<Obstacle> obstacles;
    Weapon playerWeapon;
    SaveGame saveSystem;
    AchievementSystem achievements;
    Leaderboard leaderboard;
    
    // Power-up effects
    bool hasShield;
    bool hasRapidFire;
    bool hasDoubleShot;
    Uint32 shieldEndTime;
    Uint32 rapidFireEndTime;
    Uint32 doubleShotEndTime;
    Uint32 lastFireTime;
    
    // Game stats
    int enemiesKilled;
    int powerUpsCollected;
    int shotsFired;
    int shotsHit;
    Uint32 levelStartTime;
    Uint32 damageCooldownEnd = 0;
    float enemySpawnTimer = 0.0f;

    SDL_Window *gWindow = nullptr;
    SDL_Renderer *gRenderer = nullptr;
    SDL_Texture *gSquareTexture = nullptr;
    SDL_Texture *gBackgroundTexture = nullptr;
    SDL_Texture *gBulletTexture = nullptr;
    SDL_Texture *textTexture = nullptr;
    SDL_Texture *gObstacleTexture = nullptr;
    SDL_Texture *gMenuTexture = nullptr;
    SDL_Texture *pointsTexture = nullptr;
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *gHealth = nullptr;
    SDL_Texture *loadTexture(const std::string &path);

    SDL_Rect menuRect;
    SDL_Rect gSquareRect;
    SDL_Rect dstRect = {10, 10, 200, 50};
    SDL_Rect playButtonRect;
    SDL_Rect quitButtonRect;
    SDL_Rect playAgainButtonRect;
    SDL_Rect backToMenuButtonRect;
    TTF_Font *font1 = nullptr;
    TTF_Font *menuFont = nullptr;

    std::array<SDL_Texture*, 4> obstacleTextures{};
    std::array<SDL_Texture*, 5> powerUpTextures{};
    bool sdlInitialized = false;

    Mix_Chunk *gShootSound = nullptr;
    Mix_Chunk *gCollision = nullptr;
    Mix_Chunk *gSound = nullptr;
    Mix_Chunk *gShotTrung = nullptr;
    Mix_Chunk *gGameOver = nullptr;
    Mix_Chunk *gWin = nullptr;
};

#endif // GAME_H
