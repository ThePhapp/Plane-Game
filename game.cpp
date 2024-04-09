#include "Game.h"

Game::Game()
{
    if (!initSDL())
    {
        std::cout << "SDL initialization failed!" << std::endl;
        exit(1);
    }

    gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
    gBulletRect = {SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, SCREEN_HEIGHT / 2, 50, 50};
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

    while (!quit)
    {
        while (!gameOver && !quit)
        {
            handleEvent(e, quit);
            updateObstacle();
            handleBulletCollision();
            render();
            if (isCollision() || miss == 3)
            {
                currentHealth -= 1;
                miss = 0;
                isObstacleActive = false;
                if (Mix_PlayChannel(-1, gCollision, 0) == -1) // SoundVaCham
                {
                    std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }

                if (currentHealth == 0)
                {
                    gameOver = true;
                    renderGameOver();
                    isObstacleActive = false;
                }
            }
        }

        while (gameOver && !quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_SPACE)
                    {
                        gameOver = false;
                        if (points > highestPoint)
                            highestPoint = points;
                        points = 0;
                        level = 1;
                        gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
                        gBulletRect = {SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, SCREEN_HEIGHT / 2, 50, 50};
                        currentHealth = maxHealth;
                    }
                }
            }
            SDL_RenderPresent(gRenderer);
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
    gWindow = SDL_CreateWindow("Shoot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

void Game::closeSDL()
{
    SDL_DestroyTexture(gSquareTexture);
    SDL_DestroyTexture(gBulletTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

void Game::render()
{
    SDL_RenderClear(gRenderer);
    renderBackground();
    SDL_RenderCopy(gRenderer, gSquareTexture, nullptr, &gSquareRect);
    SDL_RenderCopy(gRenderer, gObstacleTexture, nullptr, &gObstacleRect);
    renderPoints();
    renderHealth();
    if (!Mix_Playing(-1)) // SoundBackground
    {
        if (Mix_PlayChannel(-1, gSound, 0) == -1)
        {
            std::cout << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }

    if (isBulletActive)
    {
        gBulletRect.x += 4; // toc do vien dan
        SDL_RenderCopy(gRenderer, gBulletTexture, nullptr, &gBulletRect);

        if (gBulletRect.x > SCREEN_WIDTH)
        {
            isBulletActive = false;
        }
    }
    SDL_RenderPresent(gRenderer);
}

bool Game::isCollision()
{
    return Collision::checkCollision(gSquareRect, gObstacleRect);
}