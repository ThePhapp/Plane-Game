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
    gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
    gBulletRect = {SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, SCREEN_HEIGHT / 2, 50, 50};
    loadBackground();
    this->points = 0;
}

Game::~Game()
{
    closeSDL();
}

void Game::run()
{
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
                gameOver = true;
                renderGameOver();
                isObstacleActive = false;
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
                    }
                }
            }
            renderGameOver();
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
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1)
    {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
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
void Game::renderPoints() {
    std::string pointString = "Points: " + std::to_string(points);
    TTF_Font* font1 = TTF_OpenFont("font/2.ttf", 35);
    if (font1 == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font1, pointString.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font1);
        return;
    }
    SDL_Texture* pointsTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (pointsTexture == nullptr) {
        std::cerr << "Unable to create texture from text surface: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font1);

    SDL_Rect dstRect = {10, 10, textSurface->w, textSurface->h}; 
    SDL_RenderCopy(gRenderer, pointsTexture, nullptr, &dstRect);
    SDL_DestroyTexture(pointsTexture); 
}

void Game::renderGameOver() 
{
    TTF_Font* font2 = TTF_OpenFont("font/1.ttf", 60);
    if(font2 == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColorGV = {225, 0, 0};
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font2, "GameOver", textColorGV);
    if (gameOverSurface == nullptr) {
        std::cerr << "Unable to render Game Over surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font2);
        return;
    }

    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(gRenderer, gameOverSurface);
    if (gameOverTexture == nullptr) {
        std::cerr << "Unable to create texture from Game Over surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(gameOverSurface);
        TTF_CloseFont(font2);
        return;
    }
    SDL_Rect gameOverRect = {(SCREEN_WIDTH - gameOverSurface->w) / 2, (SCREEN_HEIGHT - gameOverSurface->h) / 2, gameOverSurface->w, gameOverSurface->h};

    SDL_Surface* playAgainSurface = TTF_RenderText_Solid(font2, "Press Space to PlayAgain !!!", textColorGV);
    if (playAgainSurface == nullptr) {
        std::cerr << "Unable to render Play Again surface: " << TTF_GetError() << std::endl;
        SDL_DestroyTexture(gameOverTexture);
        TTF_CloseFont(font2);
        return;
    }

    SDL_Texture* playAgainTexture = SDL_CreateTextureFromSurface(gRenderer, playAgainSurface);
    if (playAgainTexture == nullptr) {
        std::cerr << "Unable to create texture from Play Again surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(playAgainSurface);
        SDL_DestroyTexture(gameOverTexture);
        TTF_CloseFont(font2);
        return;
    }
    SDL_Rect playAgainRect = {(SCREEN_WIDTH - playAgainSurface->w) / 2, gameOverRect.y + gameOverRect.h + 20, playAgainSurface->w, playAgainSurface->h};

    SDL_RenderCopy(gRenderer, gameOverTexture, nullptr, &gameOverRect);    //render
    SDL_RenderCopy(gRenderer, playAgainTexture, nullptr, &playAgainRect);
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
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_UP:
                    gSquareRect.y -= 30;
                    if (gSquareRect.y < 0)
              {
                gSquareRect.y = 0;
              }
                    break;
                case SDLK_DOWN:
                    gSquareRect.y += 30;
                    if(gSquareRect.y > SCREEN_HEIGHT - gSquareRect.w) 
                    {
                        gSquareRect.y = SCREEN_HEIGHT - gSquareRect.w;
                    }
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
                        gBulletRect.x = gSquareRect.x + SQUARE_SIZE; 
                        gBulletRect.y = gSquareRect.y + SQUARE_SIZE / 2 - gBulletRect.h / 2;
                     }
                     break;
            }
        }
    }
}
bool Game::isCollision() {
     return Collision::checkCollision(gSquareRect , gObstacleRect);
}