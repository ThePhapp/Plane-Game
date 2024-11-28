#include "Game.h"
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
                if (currentTime - boss.explosionStartTime >= 410) // render hết boss's Explosion xong mới render win menu
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

void Game::updateFPS() // điều chỉnh fps cho game
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

    // Neu boss active, render boss va bullet cua boss
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

bool Game::isCollision() // hàm kiểm tra va chạm
{
    return Collision::checkCollision(gSquareRect, gObstacleRect);
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