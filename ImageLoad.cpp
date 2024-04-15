#include "game.h"

void Game::loadImagee()
{
    gSquareTexture = loadTexture("image/Player.png");
    gBulletTexture = loadTexture("image/Bullet.png");
    if (gSquareTexture == nullptr || gBulletTexture == nullptr)
    {
        std::cout << "Failed to load textures!" << std::endl;
        closeSDL();
    }

    gObstacleTexture = loadTexture("image/obstacle.png");
    if (gObstacleTexture == nullptr)
    {
        std::cout << "Failed to load obstacle texture!" << std::endl;
        closeSDL();
    }

    gHealth = loadTexture("image/health.png");
    if (gHealth == nullptr)
    {
        std::cout << "Failed to load health texture!" << std::endl;
        closeSDL();
    }

    // Tải hình ảnh cho boss và đạn của boss
    boss.loadBossImage(gRenderer, "image/boss.png");
    boss.loadBulletImage(gRenderer, "image/bullet.png");
}

void Game::loadBackground()
{
    gBackgroundTexture = loadTexture("image/Background.png");

    if (gBackgroundTexture == nullptr)
    {
        std::cout << "Failed to load background texture!" << std::endl;
        closeSDL();
        exit(3);
    }
    backgroundPosX = 0;
}

void Game::renderBackground() // tao background di chuyen
{
    backgroundPosX -= 1;

    if (backgroundPosX < -SCREEN_WIDTH)
    {
        backgroundPosX = 0;
    }

    SDL_Rect backgroundRect1 = {backgroundPosX, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Rect backgroundRect2 = {backgroundPosX + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, &backgroundRect1);
    SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, &backgroundRect2);
}

SDL_Texture *Game::loadTexture(const std::string &path)
{
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        std::cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr)
    {
        std::cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return texture;
}
void Game::renderPoints()
{
    std::string pointString = "Score: " + std::to_string(points) + "  Highest: " + std::to_string(highestPoint) + "  Level: " + std::to_string(level);
    TTF_Font *font1 = TTF_OpenFont("font/2.ttf", 35);
    if (font1 == nullptr)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font1, pointString.c_str(), textColor);
    if (textSurface == nullptr)
    {
        std::cout << "Unable to render text surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font1);
        return;
    }
    SDL_Texture *pointsTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (pointsTexture == nullptr)
    {
        std::cout << "Unable to create texture from text surface: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font1);

    SDL_Rect dstRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(gRenderer, pointsTexture, nullptr, &dstRect);
    SDL_DestroyTexture(pointsTexture);
}

void Game::loadSoundd() // load sound
{
    gShootSound = Mix_LoadWAV("sound/shoot.wav");
    if (gShootSound == nullptr)
    {
        std::cout << "Failed to load ShootSound! Error: " << Mix_GetError() << std::endl;
    }
    gSound = Mix_LoadWAV("Sound/mission.wav");
    if (gSound == nullptr)
    {
        std::cout << "Failed to load gSound! Error: " << Mix_GetError() << std::endl;
    }
    gShotTrung = Mix_LoadWAV("Sound/gg.wav");
    if (gShotTrung == nullptr)
    {
        std::cout << "Failed to load gShotTrung! Error: " << Mix_GetError() << std::endl;
    }
    gCollision = Mix_LoadWAV("Sound/vacham.wav");
    if (gCollision == nullptr)
    {
        std::cout << "Failed to load gCollision! Error: " << Mix_GetError() << std::endl;
    }
    gGameOver = Mix_LoadWAV("Sound/GameOver.wav");
    if (gGameOver == nullptr)
    {
        std::cout << "Failed to load gGameOver! Error: " << Mix_GetError() << std::endl;
    }
}

void Game::renderHealth()
{
    int heartX = 10;
    int heartY = SCREEN_HEIGHT - HEALTH_HEIGHT - 10;

    for (int i = 0; i < maxHealth; ++i)
    {
        SDL_Rect destRect = {heartX, heartY, HEALTH_WIDTH, HEALTH_HEIGHT};
        if (i < currentHealth)
        {
            SDL_RenderCopy(gRenderer, gHealth, nullptr, &destRect);
        }
        heartX += HEALTH_WIDTH + HEALTH_SPACING;
    }
}

void Game::renderGameOver()
{
    SDL_Surface *gameOverSurface = IMG_Load("image/1.png");
    if (gameOverSurface)
    {
        SDL_Texture *gameOverTexture = SDL_CreateTextureFromSurface(gRenderer, gameOverSurface);
        SDL_FreeSurface(gameOverSurface);
        SDL_Rect gameOverRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(gRenderer, gameOverTexture, nullptr, &gameOverRect);
        SDL_DestroyTexture(gameOverTexture);
    }
    else
    {
        std::cout << "Failed to load game over image. SDL_Image Error: " << IMG_GetError() << std::endl;
    }
    SDL_RenderPresent(gRenderer);
}

void Game::handleGameOverEvent(SDL_Event &e, bool &quit, bool &gameOver)
{
    int mouseX, mouseY;
    playAgainButtonRect = {SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT * 3 / 4 - 50, 400, 90};
    backToMenuButtonRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT * 3 / 4 + 60, 200, 80};

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= playAgainButtonRect.x && mouseX <= playAgainButtonRect.x + playAgainButtonRect.w &&
                mouseY >= playAgainButtonRect.y && mouseY <= playAgainButtonRect.y + playAgainButtonRect.h)
            {
                gameOver = false;
                if (points > highestPoint)
                    highestPoint = points;
                points = 0;
                level = 1;
                boss.setActive(false);
                boss.setHealth(100);
                currentHealth == maxHealth;
                gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
                for (auto &bullet : bullets)
                {
                    bullet.rect = {SCREEN_WIDTH / 2 - SQUARE_SIZE / 2, SCREEN_HEIGHT / 2, 50, 50};
                }
                currentHealth = maxHealth;
            }
            else if (mouseX >= backToMenuButtonRect.x && mouseX <= backToMenuButtonRect.x + backToMenuButtonRect.w &&
                     mouseY >= backToMenuButtonRect.y && mouseY <= backToMenuButtonRect.y + backToMenuButtonRect.h)
            {
                gameOver = false;
                showMainMenu = true;
            }
        }
    }
}

void Game::loadMenu()
{
    SDL_Surface *surface = IMG_Load("image/Menu.png");
    if (surface)
    {
        gMenuTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);

        // Đặt vị trí cho menu
        menuRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    }
    else
    {
        std::cout << "Failed to load menu image. SDL_Image Error: " << IMG_GetError() << std::endl;
    }
}

void Game::handleMainMenuEvent(SDL_Event &e, bool &quit, bool &showMainMenu)
{
    int mouseX, mouseY;
    playButtonRect = {SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT * 5 / 7 + 40, 150, 70};
    quitButtonRect = {SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT * 5 / 7 + 130, 320, 80};
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
            showMainMenu = false;
        }
        else if (e.type == SDL_MOUSEMOTION)
        { // Bắt sự kiện di chuyển chuột
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= playButtonRect.x && mouseX <= playButtonRect.x + playButtonRect.w &&
                mouseY >= playButtonRect.y && mouseY <= playButtonRect.y + playButtonRect.h)
            {
                mouseOverPlay = true;
            }
            else if (mouseX >= quitButtonRect.x && mouseX <= quitButtonRect.x + quitButtonRect.w &&
                     mouseY >= quitButtonRect.y && mouseY <= quitButtonRect.y + quitButtonRect.h)
            {
                mouseOverQuit = true;
            }
            else
            {
                mouseOverPlay = false;
                mouseOverQuit = false;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= playButtonRect.x && mouseX <= playButtonRect.x + playButtonRect.w &&
                mouseY >= playButtonRect.y && mouseY <= playButtonRect.y + playButtonRect.h)
            {
                showMainMenu = false; // bắt đầu game
                currentHealth = maxHealth;
                if (points > highestPoint)
                    highestPoint = points;
                points = 0;
                level = 1;
                boss.setActive(false);
                boss.setHealth(100);
                currentHealth == maxHealth;
                gSquareRect = {0, SCREEN_HEIGHT / 2, SQUARE_SIZE, SQUARE_SIZE};
            }
            else if (mouseX >= quitButtonRect.x && mouseX <= quitButtonRect.x + quitButtonRect.w &&
                     mouseY >= quitButtonRect.y && mouseY <= quitButtonRect.y + quitButtonRect.h)
            {
                quit = true; // thoát game
                showMainMenu = false;
            }
        }
    }
}

void Game::renderMainMenu()
{
    SDL_RenderCopy(gRenderer, gMenuTexture, nullptr, &menuRect);

    // Vẽ Play và Quit button
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    // Vẽ Play button
    if (mouseOverPlay)
    {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
    }
    SDL_RenderDrawRect(gRenderer, &playButtonRect);

    // Vẽ Quit button
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    if (mouseOverQuit)
    {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
    }
    SDL_RenderDrawRect(gRenderer, &quitButtonRect);

    SDL_RenderPresent(gRenderer);
}