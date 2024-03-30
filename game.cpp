#include "Game.h"

Game::Game()
{
    if (!initSDL())
    {
        std::cerr << "SDL initialization failed!" << std::endl;
        exit(1);
    }
    
    gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
    gBulletRect = {SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, SCREEN_HEIGHT / 2, 50, 50};
    loadImagee();
    loadBackground();
    loadSoundd();
    this->points = 0;
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
        while(!gameOver && !quit)
        {
            handleEvent(e, quit);
            updateObstacle(); 
            handleBulletCollision();
            render();
            if(isCollision()) {
                currentHealth -= 1;
                isObstacleActive = false;
                 if (Mix_PlayChannel(-1, gCollision, 0) == -1)    //SoundVaCham
                    {
                        std::cerr << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    }
  
                if(currentHealth == 0) {
                    gameOver = true;
                    renderGameOver();
                    isObstacleActive = false;
                }
            }
        }
        
        while(gameOver && !quit)
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
                        points = 0;
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
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    gWindow = SDL_CreateWindow("Shoot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))     //init sdl_image
    {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1)    //init sdl_ttf
    {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)    //init sdl_mixer
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
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
    SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, nullptr);
    SDL_RenderCopy(gRenderer, gSquareTexture, nullptr, &gSquareRect);
    SDL_RenderCopy(gRenderer, gObstacleTexture, nullptr, &gObstacleRect);
    renderPoints();
    renderHealth();
    if (!Mix_Playing(-1))      //SoundBackground
    {
        if (Mix_PlayChannel(-1, gSound, 0) == -1)
        {
            std::cerr << "Failed to play sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
    }
    
    if (isBulletActive)
{
    gBulletRect.x += 4;     //toc do vien dan
    SDL_RenderCopy(gRenderer, gBulletTexture, nullptr, &gBulletRect);

    if (gBulletRect.x > SCREEN_WIDTH)
    {
        isBulletActive = false;
    }
}
    SDL_RenderPresent(gRenderer);
}

void Game::handleEvent(SDL_Event& e, bool& quit)
{
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
    }
    
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_ESCAPE])
    {
        quit = true;
    }

    if (currentKeyStates[SDL_SCANCODE_UP])
    {
        gSquareRect.y -= 2;
        if (gSquareRect.y < 0)
        {
            gSquareRect.y = 0;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        gSquareRect.y += 2;
        if(gSquareRect.y > SCREEN_HEIGHT - gSquareRect.w) 
        {
            gSquareRect.y = SCREEN_HEIGHT - gSquareRect.w;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        gSquareRect.x -= 2;
        if (gSquareRect.x < 0) 
        {
            gSquareRect.x = 0;
        }
    }
    
    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        gSquareRect.x += 2;
        if(gSquareRect.x > SCREEN_WIDTH - gSquareRect.w) 
        {
            gSquareRect.x = SCREEN_WIDTH - gSquareRect.w;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_SPACE])
    {
        if (!isBulletActive)
        {
            isBulletActive = true;
            gBulletRect.x = gSquareRect.x + SQUARE_SIZE; 
            gBulletRect.y = gSquareRect.y + SQUARE_SIZE / 2 - gBulletRect.h / 2;
            
            if (Mix_PlayChannel(-1, gShootSound, 0) == -1)
            {
                std::cerr << "Failed to play shoot sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
            }
            
        }
    }
}

bool Game::isCollision() {
     return Collision::checkCollision(gSquareRect , gObstacleRect);
}