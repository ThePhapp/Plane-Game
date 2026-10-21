#include "Game.h"
#include "Control.h"
#include "BulletObject/Bullet.h"
#include "ExplosionObject/ExplosionObject.h"
#include "BossObject/BossObject.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>

std::vector<Bullet> Game::bullets;

Game::Game()
{
    if (!initSDL())
    {
        std::cout << "SDL initialization failed!" << std::endl;
        exit(1);
    }

    gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
    loadImagee();
    loadBackground();
    loadSoundd();
    this->points = 0;
    currentHealth = maxHealth;
    
    // Initialize new systems
    hasShield = false;
    hasRapidFire = false;
    hasDoubleShot = false;
    shieldEndTime = 0;
    rapidFireEndTime = 0;
    doubleShotEndTime = 0;
    lastFireTime = 0;
    enemiesKilled = 0;
    powerUpsCollected = 0;
    shotsFired = 0;
    shotsHit = 0;
    levelStartTime = SDL_GetTicks();
    
    // Initialize systems
    achievements.initialize();
    leaderboard.loadFromFile("leaderboard.dat");
    loadGame();
}

Game::~Game()
{
    closeSDL();
}

void Game::resetRun()
{
    points = 0;
    currentHealth = maxHealth;
    enemiesKilled = 0;
    powerUpsCollected = 0;
    shotsFired = 0;
    shotsHit = 0;
    hasShield = false;
    hasRapidFire = false;
    hasDoubleShot = false;
    shieldEndTime = 0;
    rapidFireEndTime = 0;
    doubleShotEndTime = 0;
    lastFireTime = 0;
    damageCooldownEnd = 0;
    enemySpawnTimer = 0.0f;
    levelStartTime = SDL_GetTicks();

    bullets.clear();
    Boss::getBulletBoss().clear();
    powerUps.clear();
    obstacles.clear();
    boss.reset();
    exp.stop();
}

void Game::damagePlayer()
{
    const Uint32 now = SDL_GetTicks();
    if (now < damageCooldownEnd)
        return;

    damageCooldownEnd = now + 350;
    if (!hasShield)
    {
        currentHealth = std::max(0, currentHealth - 1);
        if (Mix_PlayChannel(-1, gCollision, 0) == -1)
            std::cout << "Failed to play collision sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    if (!exp.isExploding())
        exp.start(gSquareRect.x, gSquareRect.y);
}

void Game::run()
{
    srand(time(NULL));
    SDL_Event e;
    bool quit = false;
    bool gameOver = false;
    Uint32 previousFrameTicks = SDL_GetTicks();

    loadMenu();

    while (!quit)
    {
        while (showMainMenu && !quit)
        {
            handleMainMenuEvent(e, quit, showMainMenu);
            renderMainMenu();
        }

        if (!quit)
        {
            showLevelMenu();
        }

        while (!gameOver && !quit)
        {
            const Uint32 frameStartTicks = SDL_GetTicks();
            const float deltaSeconds = std::min(
                (frameStartTicks - previousFrameTicks) / 1000.0f,
                0.05f);
            previousFrameTicks = frameStartTicks;

            handleEvent(e, quit, deltaSeconds);
            
            // Update new systems
            updateBullets(deltaSeconds);
            updateObstacles(deltaSeconds);
            updatePowerUps(deltaSeconds);
            handleObstacleCollision();
            handlePowerUpCollision();
            handleBossBulletCollision();
            
            // Spawn enemies on a time-based schedule instead of per-frame randomness.
            enemySpawnTimer += deltaSeconds;
            const float spawnInterval = std::max(0.35f, 1.15f - level * 0.2f);
            if (obstacles.size() < 5 && enemySpawnTimer >= spawnInterval)
            {
                enemySpawnTimer = 0.0f;
                spawnObstacleNew();
            }

            if (!boss.isActive() && boss.getHealth() > 0 && points >= 1000)
                boss.setActive(true);

            if (boss.isActive())
            {
                boss.update(deltaSeconds);
                if (boss.handleBulletCollision())
                    boss.startExplosion(gRenderer);
            }
            
            render(deltaSeconds);

            const Uint32 frameTicks = SDL_GetTicks() - frameStartTicks;
            if (frameTicks < SCREEN_TICKS_PER_FRAME)
                SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);

            if (currentHealth <= 0)
            {
                gameOver = true;
                renderGameOver();
                
                // Save score to leaderboard
                ScoreEntry entry("Player", points, level);
                leaderboard.addScore(entry);
                saveGame();
            }

            if (!boss.isActive() && boss.getHealth() == 0 && points >= 1000)
            {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - boss.getExplosionStartTime() >= 410)
                {
                    gameOver = true;
                    renderWinMenu();
                    
                    // Achievement for beating boss
                    achievements.unlock(ACH_BOSS_SLAYER);
                    
                    // Save score
                    ScoreEntry entry("Player", points, level);
                    leaderboard.addScore(entry);
                    saveGame();
                }
            }
        }

        while (gameOver && !quit)
        {
            handleGameOverEvent(e, quit, gameOver);
        }
    }
}

bool Game::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow("War's Plane", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr)
    {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr)
    {
        std::cout << "Accelerated renderer unavailable, using software renderer: "
                  << SDL_GetError() << std::endl;
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
        if (gRenderer == nullptr)
        {
            std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
    }
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) // init sdl_image
    {
        std::cout << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) // init sdl_ttf
    {
        std::cout << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) // init sdl_mixer
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    sdlInitialized = true;
    return true;
}

void Game::render(float deltaSeconds)
{
    SDL_RenderClear(gRenderer);
    renderBackground(deltaSeconds);
    
    // Render player
    SDL_RenderCopy(gRenderer, gSquareTexture, nullptr, &gSquareRect);
    
    // Render new obstacles
    renderObstacles();
    
    // Render power-ups
    renderPowerUps();
    
    // Render UI
    renderPoints();
    renderHealth();
    
    // Render bullets
    renderBullets();
    
    boss.render(gRenderer);

    loadExplosion();
    
    // Render achievement notifications
    achievements.render(gRenderer, font1);

    if (!Mix_Playing(-1))
    {
        if (Mix_PlayChannel(-1, gSound, 0) == -1)
        {
            std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }

    SDL_RenderPresent(gRenderer);
}

void Game::showLevelMenu()
{
    bool choosing = true;
    SDL_Event e;
    int selectedLevel = 1;
    TTF_Font* font = TTF_OpenFont("font/1.ttf", 36);

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color borderColor = {80, 80, 80, 255};

    while (choosing)
    {
        SDL_SetRenderDrawColor(gRenderer, 30, 30, 30, 255);
        SDL_RenderClear(gRenderer);

        SDL_Surface* titleSurface = TTF_RenderUTF8_Blended(font, "CHOOSE YOUR LEVEL", textColor);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(gRenderer, titleSurface);
        int tw, th;
        SDL_QueryTexture(titleTexture, NULL, NULL, &tw, &th);
        SDL_Rect titleRect = {SCREEN_WIDTH/2 - tw/2, SCREEN_HEIGHT/2 - 160, tw, th};
        SDL_RenderCopy(gRenderer, titleTexture, NULL, &titleRect);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(titleTexture);

        SDL_Rect easyRect   = {SCREEN_WIDTH/2 - 220, SCREEN_HEIGHT/2 - 60, 140, 80};
        SDL_Rect mediumRect = {SCREEN_WIDTH/2 - 70,  SCREEN_HEIGHT/2 - 60, 140, 80};
        SDL_Rect hardRect   = {SCREEN_WIDTH/2 + 80,  SCREEN_HEIGHT/2 - 60, 140, 80};

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // Hiệu ứng hover
        bool hoverEasy   = (mx >= easyRect.x && mx <= easyRect.x + easyRect.w && my >= easyRect.y && my <= easyRect.y + easyRect.h);
        bool hoverMedium = (mx >= mediumRect.x && mx <= mediumRect.x + mediumRect.w && my >= mediumRect.y && my <= mediumRect.y + mediumRect.h);
        bool hoverHard   = (mx >= hardRect.x && mx <= hardRect.x + hardRect.w && my >= hardRect.y && my <= hardRect.y + hardRect.h);
        SDL_SetRenderDrawColor(gRenderer, hoverEasy ? 0 : 0, hoverEasy ? 255 : 200, 0, 255);
        SDL_RenderFillRect(gRenderer, &easyRect);
        SDL_SetRenderDrawColor(gRenderer, hoverMedium ? 255 : 200, hoverMedium ? 255 : 200, 0, 255);
        SDL_RenderFillRect(gRenderer, &mediumRect);
        SDL_SetRenderDrawColor(gRenderer, hoverHard ? 255 : 200, 0, 0, 255);
        SDL_RenderFillRect(gRenderer, &hardRect);

        SDL_SetRenderDrawColor(gRenderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_RenderDrawRect(gRenderer, &easyRect);
        SDL_RenderDrawRect(gRenderer, &mediumRect);
        SDL_RenderDrawRect(gRenderer, &hardRect);

        struct ButtonLabel { const char* text; SDL_Rect rect; };
        ButtonLabel labels[] = {
            {"Easy", easyRect},
            {"Medium", mediumRect},
            {"Hard", hardRect}
        };
        for (auto& label : labels) {
            SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, label.text, textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
            int tw, th;
            SDL_QueryTexture(textTexture, NULL, NULL, &tw, &th);
            SDL_Rect textRect = {label.rect.x + (label.rect.w-tw)/2, label.rect.y + (label.rect.h-th)/2, tw, th};
            SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        SDL_RenderPresent(gRenderer);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                exit(0);
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (hoverEasy)   { selectedLevel = 1; choosing = false; }
                if (hoverMedium) { selectedLevel = 2; choosing = false; }
                if (hoverHard)   { selectedLevel = 3; choosing = false; }
            }
        }
    }
    TTF_CloseFont(font);
    level = selectedLevel;
    applyLevelSettings();
}

void Game::applyLevelSettings()
{
    switch (level)
    {
    case 1: // Easy - Normal Boss
        boss.setType(BOSS_NORMAL);
        boss.setHealth(80);
        boss.setSpeed(3);
        boss.setBulletSpeed(7);
        break;
        
    case 2: // Medium - Fast Boss
        boss.setType(BOSS_FAST);
        boss.setHealth(120);
        boss.setSpeed(5);
        boss.setBulletSpeed(10);
        break;
        
    case 3: // Hard - Ultimate Boss
        boss.setType(BOSS_ULTIMATE);
        boss.setHealth(180);
        boss.setSpeed(8);
        boss.setBulletSpeed(15);
        break;
    }
}

void Game::closeSDL()
{
    if (!sdlInitialized)
        return;

    boss.releaseResources();
    exp.release();

    if (font1)
    {
        TTF_CloseFont(font1);
        font1 = nullptr;
    }
    if (menuFont)
    {
        TTF_CloseFont(menuFont);
        menuFont = nullptr;
    }

    for (SDL_Texture*& texture : powerUpTextures)
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
    for (SDL_Texture*& texture : obstacleTextures)
    {
        if (texture && texture != gObstacleTexture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

    SDL_DestroyTexture(gSquareTexture);
    SDL_DestroyTexture(gBackgroundTexture);
    SDL_DestroyTexture(gBulletTexture);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(gObstacleTexture);
    SDL_DestroyTexture(gHealth);
    SDL_DestroyTexture(gMenuTexture);
    gSquareTexture = nullptr;
    gBackgroundTexture = nullptr;
    gBulletTexture = nullptr;
    textTexture = nullptr;
    gObstacleTexture = nullptr;
    gHealth = nullptr;
    gMenuTexture = nullptr;

    Mix_FreeChunk(gShootSound);
    Mix_FreeChunk(gCollision);
    Mix_FreeChunk(gSound);
    Mix_FreeChunk(gShotTrung);
    Mix_FreeChunk(gGameOver);
    Mix_FreeChunk(gWin);
    gShootSound = nullptr;
    gCollision = nullptr;
    gSound = nullptr;
    gShotTrung = nullptr;
    gGameOver = nullptr;
    gWin = nullptr;

    if (gRenderer)
    {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = nullptr;
    }
    if (gWindow)
    {
        SDL_DestroyWindow(gWindow);
        gWindow = nullptr;
    }

    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
    sdlInitialized = false;
}

// ==================== POWER-UP FUNCTIONS ====================

void Game::spawnPowerUp()
{
    int x = SCREEN_WIDTH;
    int y = rand() % (SCREEN_HEIGHT - 100) + 50;
    PowerUpType type = static_cast<PowerUpType>(rand() % 5);

    powerUps.emplace_back(x, y, type);
    powerUps.back().setTexture(powerUpTextures[type]);
}

void Game::updatePowerUps(float deltaSeconds)
{
    for (auto& powerUp : powerUps)
    {
        if (powerUp.isActive())
            powerUp.update(deltaSeconds);
    }
    
    // Remove inactive power-ups
    powerUps.erase(
        std::remove_if(powerUps.begin(), powerUps.end(),
                      [](const PowerUp& p) { return !p.isActive(); }),
        powerUps.end());
    
    // Check power-up timers
    Uint32 currentTime = SDL_GetTicks();
    if (hasShield && currentTime > shieldEndTime)
        hasShield = false;
    if (hasRapidFire && currentTime > rapidFireEndTime)
        hasRapidFire = false;
    if (hasDoubleShot && currentTime > doubleShotEndTime)
        hasDoubleShot = false;
}

void Game::handlePowerUpCollision()
{
    for (auto& powerUp : powerUps)
    {
        if (powerUp.isActive())
        {
            SDL_Rect powerUpRect = powerUp.getRect();
            if (SDL_HasIntersection(&gSquareRect, &powerUpRect))
            {
                applyPowerUpEffect(powerUp.getType());
                powerUp.setActive(false);
                powerUpsCollected++;
                achievements.updateProgress(ACH_COLLECTOR, 1);
            }
        }
    }
}

void Game::applyPowerUpEffect(PowerUpType type)
{
    Uint32 currentTime = SDL_GetTicks();
    
    switch(type)
    {
        case POWERUP_SHIELD:
            hasShield = true;
            shieldEndTime = currentTime + 8000;
            std::cout << "Shield activated!" << std::endl;
            break;
            
        case POWERUP_RAPID_FIRE:
            hasRapidFire = true;
            rapidFireEndTime = currentTime + 10000;
            std::cout << "Rapid Fire activated!" << std::endl;
            break;
            
        case POWERUP_DOUBLE_SHOT:
            hasDoubleShot = true;
            doubleShotEndTime = currentTime + 12000;
            std::cout << "Double Shot activated!" << std::endl;
            break;
            
        case POWERUP_HEALTH:
            if (currentHealth < maxHealth)
            {
                currentHealth++;
                std::cout << "Health restored!" << std::endl;
            }
            break;
            
        case POWERUP_BOMB:
            // Destroy all enemies on screen
            for (auto& obstacle : obstacles)
            {
                if (obstacle.isActive())
                {
                    obstacle.setActive(false);
                    points += obstacle.getPoints();
                    enemiesKilled++;
                }
            }
            std::cout << "Bomb activated! All enemies destroyed!" << std::endl;
            break;
    }
}

void Game::renderPowerUps()
{
    for (auto& powerUp : powerUps)
    {
        if (powerUp.isActive())
            powerUp.render(gRenderer);
    }
}

// ==================== OBSTACLE FUNCTIONS ====================

void Game::spawnObstacleNew()
{
    // Random enemy type based on level
    EnemyType type;
    int chance = rand() % 100;
    
    if (level == 1)
    {
        type = (chance < 80) ? ENEMY_BASIC : ENEMY_FAST;
    }
    else if (level == 2)
    {
        if (chance < 50) type = ENEMY_BASIC;
        else if (chance < 80) type = ENEMY_FAST;
        else type = ENEMY_TANK;
    }
    else // level 3
    {
        if (chance < 30) type = ENEMY_BASIC;
        else if (chance < 60) type = ENEMY_FAST;
        else if (chance < 85) type = ENEMY_TANK;
        else type = ENEMY_KAMIKAZE;
    }
    
    int x = SCREEN_WIDTH;
    int y = rand() % (SCREEN_HEIGHT - 150) + 50;
    
    Obstacle obstacle(x, y, type);
    
    obstacle.setTexture(obstacleTextures[type]);
    obstacles.push_back(obstacle);
}

void Game::updateObstacles(float deltaSeconds)
{
    for (auto& obstacle : obstacles)
    {
        if (obstacle.isActive())
            obstacle.update(gSquareRect, deltaSeconds);
    }
    
    // Remove inactive obstacles
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
                      [](const Obstacle& o) { return !o.isActive(); }),
        obstacles.end());
}

void Game::handleObstacleCollision()
{
    for (auto& obstacle : obstacles)
    {
        if (!obstacle.isActive())
            continue;
            
        // Check bullet collision
        for (auto& bullet : bullets)
        {
            SDL_Rect obstacleRect = obstacle.getRect();
            if (bullet.active && SDL_HasIntersection(&bullet.rect, &obstacleRect))
            {
                bullet.active = false;
                shotsHit++;
                
                if (obstacle.takeDamage(playerWeapon.getDamage()))
                {
                    // Enemy destroyed
                    obstacle.setActive(false);
                    points += obstacle.getPoints();
                    enemiesKilled++;
                    
                    // Check achievements
                    if (enemiesKilled == 1)
                        achievements.unlock(ACH_FIRST_BLOOD);
                    if (enemiesKilled >= 100)
                        achievements.unlock(ACH_DESTROYER);
                    
                    // Chance to spawn power-up
                    if (rand() % 100 < 15)
                        spawnPowerUp();
                }
            }
        }
        
        // Check player collision
        SDL_Rect obstacleRect = obstacle.getRect();
        if (obstacle.isActive() && SDL_HasIntersection(&gSquareRect, &obstacleRect))
        {
            obstacle.setActive(false);
            damagePlayer();
        }
    }
}

void Game::renderObstacles()
{
    for (auto& obstacle : obstacles)
    {
        if (obstacle.isActive())
            obstacle.render(gRenderer);
    }
}

// ==================== SAVE/LOAD FUNCTIONS ====================

void Game::saveGame()
{
    GameData data;
    highestPoint = std::max(highestPoint, points);
    data.highScore = highestPoint;
    data.coins = points;
    data.level = level;
    data.weaponLevel = playerWeapon.getLevel();
    
    // Save achievements
    for (int i = 0; i < 10; i++)
    {
        if (i < static_cast<int>(achievements.getAchievements().size()))
            data.achievements[i] = achievements.getAchievements()[i].unlocked;
        else
            data.achievements[i] = false;
    }
    
    saveSystem.saveProgress("savegame.dat", data);
    leaderboard.saveToFile("leaderboard.dat");
}

void Game::loadGame()
{
    GameData data;
    if (saveSystem.loadProgress("savegame.dat", data))
    {
        highestPoint = data.highScore;
        
        // Load weapon level
        for (int i = 1; i < data.weaponLevel; i++)
            playerWeapon.upgrade();
        
        // Load achievements
        for (int i = 0; i < 10 && i < static_cast<int>(achievements.getAchievements().size()); i++)
        {
            if (data.achievements[i])
                achievements.unlock(static_cast<AchievementID>(i));
        }
    }
}

void Game::showLeaderboard()
{
    leaderboard.render(gRenderer, font1);
}

void Game::showAchievements()
{
    achievements.render(gRenderer, font1);
}
