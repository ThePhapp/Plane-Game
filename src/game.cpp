#include "Game.h"
#include "Control.h"
#include "BulletObject/Bullet.h"
#include "ExplosionObject/ExplosionObject.h"
#include "BossObject/BossObject.h"
#include "Collision.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

std::vector<Bullet> Game::bullets;

Game::Game()
{
    if (!initSDL())
    {
        std::cout << "SDL initialization failed!" << std::endl;
        exit(1);
    }

    gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
    for (auto &bullet : bullets)
    {
        bullet.rect = {SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, SCREEN_HEIGHT / 2, 50, 50};
    }
    loadImagee();
    loadBackground();
    loadSoundd();
    this->points = 0;
    this->miss = 0;
    currentHealth = maxHealth;
}

Game::~Game()
{
    closeSDL();
}

void Game::run()
{
    srand(time(NULL));
    SDL_Event e;
    bool quit = false;
    bool gameOver = false;

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
            handleEvent(e, quit);
            updateObstacle();
            handleBulletCollision();
            handleBossBulletCollision();
            render();

            if (isCollision() || miss == 4)
            {
                currentHealth -= 1;
                miss = 0;
                isObstacleActive = false;
                if (Mix_PlayChannel(-1, gCollision, 0) == -1) // SoundVaCham
                {
                    std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }
            }

            if (currentHealth == 0)
            {
                gameOver = true;
                renderGameOver();
                isObstacleActive = false;
            }

            if (!boss.isActive() && points > 1000)
            {
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - boss.getExplosionStartTime() >= 410)
                {
                    gameOver = true;
                    renderWinMenu();
                    isObstacleActive = false;
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
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
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

    return true;
}

void Game::updateFPS()
{
    Uint32 frameTicks = SDL_GetTicks() - gStartTime;
    if (frameTicks < SCREEN_TICKS_PER_FRAME)
    {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
    // Cập nhật thời gian bắt đầu
    gStartTime = SDL_GetTicks();
}

void Game::render()
{
    SDL_RenderClear(gRenderer);
    renderBackground();
    updateFPS();
    SDL_RenderCopy(gRenderer, gSquareTexture, nullptr, &gSquareRect);
    SDL_RenderCopy(gRenderer, gObstacleTexture, nullptr, &gObstacleRect);
    renderPoints();
    renderHealth();
    updateBullets();
    if (!boss.isActive() && points == 1000)
    {
        boss.setActive(true);
    }

    if (boss.isActive())
    {
        boss.update();
        boss.render(gRenderer);
        boss.handleBulletCollision(); // Xy ly va cham boss va bullet
    }

    loadExplosion();

    boss.loadExplosionBoss(gRenderer);

    if (!Mix_Playing(-1)) // SoundBackground
    {
        if (Mix_PlayChannel(-1, gSound, 0) == -1)
        {
            std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }

    SDL_RenderPresent(gRenderer);
}

bool Game::isCollision()
{
    return Collision::checkCollision(gSquareRect, gObstacleRect);
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
    case 1: // Easy
        boss.setHealth(80);
        boss.setSpeed(3);
        boss.setBulletSpeed(7);
        break;
    case 2: // Medium
        boss.setHealth(120);
        boss.setSpeed(5);
        boss.setBulletSpeed(10);
        break;
    case 3: // Hard
        boss.setHealth(180);
        boss.setSpeed(8);
        boss.setBulletSpeed(15);
        break;
    }
}

void Game::closeSDL()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    SDL_DestroyTexture(gSquareTexture);
    SDL_DestroyTexture(gBackgroundTexture);
    SDL_DestroyTexture(gBulletTexture);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(gObstacleTexture);
    SDL_DestroyTexture(gBulletTexture);
    SDL_DestroyTexture(gMenuTexture);

    Mix_FreeChunk(gShootSound);
    Mix_FreeChunk(gCollision);
    Mix_FreeChunk(gSound);
    Mix_FreeChunk(gShotTrung);
    Mix_FreeChunk(gGameOver);
    SDL_DestroyTexture(gSquareTexture);
    SDL_DestroyTexture(gBulletTexture);
    SDL_DestroyTexture(gMenuTexture);

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
}