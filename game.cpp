#include "Game.h"

Game::Game()
{
    if (!initSDL())
    {
        std::cerr << "SDL initialization failed!" << std::endl;
        exit(1);
    }

    gSquareTexture = loadTexture("image/Player.png");
    gBulletTexture = loadTexture("image/Bullet.png");

    if (gSquareTexture == nullptr || gBulletTexture == nullptr)
    {
        std::cerr << "Failed to load textures!" << std::endl;
        closeSDL();
        exit(2);
    }
    gObstacleTexture = loadTexture("image/obstacle.png");
    if (gObstacleTexture == nullptr)
    {
        std::cerr << "Failed to load obstacle texture!" << std::endl;
        closeSDL();
        exit(3);
    }
    gSquareRect = {SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, SCREEN_HEIGHT - SQUARE_SIZE , SQUARE_SIZE, SQUARE_SIZE};
    gBulletRect = {0, 0, 50, 50};
    loadBackground();
}

Game::~Game()
{
    closeSDL();
}

void Game::run()
{
    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        handleEvent(e, quit);
        updateObstacle(); 
        handleBulletCollision();
        render();
        if(isCollision()) {
        quit = true;
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
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}
void Game::loadBackground()
{
    gBackgroundTexture = loadTexture("image/Background.png");

    if (gBackgroundTexture == nullptr)
    {
        std::cerr << "Failed to load background texture!" << std::endl;
        closeSDL();
        exit(3);
    }
}
SDL_Texture* Game::loadTexture(const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        std::cerr << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr)
    {
        std::cerr << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return texture;
}
void Game::closeSDL()
{
    SDL_DestroyTexture(gSquareTexture);
    SDL_DestroyTexture(gBulletTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    IMG_Quit();
    SDL_Quit();
}
void Game::updateObstacle()
{
    if (!isObstacleActive)
    {
        spawnObstacle();
    }

    // Di chuyển vật vản
    gObstacleRect.y += 1;

    if (gObstacleRect.y > SCREEN_HEIGHT)
    {
        isObstacleActive = false;
    }
}

void Game::spawnObstacle()
{
    // Sinh vật cản
    gObstacleRect = {rand() % (SCREEN_WIDTH - 50), 0, 50, 50};
    isObstacleActive = true;
}

void Game::handleBulletCollision()
{
    // Đạn trùng thì dừng
    if (isBulletActive && SDL_HasIntersection(&gBulletRect, &gObstacleRect))
    {
        isBulletActive = false;
        isObstacleActive = false;
    }
}
void Game::render()
{
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, nullptr);
    SDL_RenderCopy(gRenderer, gSquareTexture, nullptr, &gSquareRect);
    SDL_RenderCopy(gRenderer, gObstacleTexture, nullptr, &gObstacleRect);
    

    if (isBulletActive)
    {
        gBulletRect.y -= 3;
        SDL_RenderCopy(gRenderer, gBulletTexture, nullptr, &gBulletRect);

        if (gBulletRect.y + gBulletRect.h < 0)
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
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_LEFT:
                    gSquareRect.x -= 25;
                    if (gSquareRect.x < 0) {
                        gSquareRect.x = 0;
                    }
                    break;
                case SDLK_RIGHT:
                    gSquareRect.x += 25;
                    if(gSquareRect.x > SCREEN_WIDTH - gSquareRect.w) 
                    {
                        gSquareRect.x = SCREEN_WIDTH - gSquareRect.w;
                    }
                    break;
                case SDLK_SPACE:
                    if (!isBulletActive)
                    {
                        isBulletActive = true;
                        gBulletRect.x = gSquareRect.x + (SQUARE_SIZE - gBulletRect.w) / 2;
                        gBulletRect.y = gSquareRect.y - gBulletRect.h;
                    }
                    break;
            }
        }
    }
}
bool Game::isCollision() {
    return Collision::checkCollision(gSquareRect, gObstacleRect);
}