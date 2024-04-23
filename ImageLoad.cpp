#include "game.h"

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
    boss.loadBulletImage(gRenderer, "image/BossBullet.png");
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

void Game::renderPoints()
{
    std::string pointString = "     Score: " + std::to_string(points) + "           Highest: " + std::to_string(highestPoint) + "                                                Level: " + std::to_string(level) +
                              "          Miss: " + std::to_string(miss) + "/4";
    font1 = TTF_OpenFont("font/2.ttf", 35);
    if (font1 == nullptr)
    {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }
    SDL_Color textColor = {255, 255, 255};
    textSurface = TTF_RenderText_Solid(font1, pointString.c_str(), textColor);
    if (textSurface == nullptr)
    {
        std::cout << "Unable to render text surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font1);
        return;
    }
    pointsTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (pointsTexture == nullptr)
    {
        std::cout << "Unable to create texture from text surface: " << SDL_GetError() << std::endl;
    }
    SDL_Rect dstRect = {10, 10, textSurface->w, textSurface->h};
    SDL_RenderCopy(gRenderer, pointsTexture, nullptr, &dstRect);

    TTF_CloseFont(font1);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(pointsTexture);
}

void Game::loadSoundd() // load sound
{
    for (int i = 0; i < Mix_AllocateChannels(-1); ++i)
    {
        Mix_Volume(i, MIX_MAX_VOLUME / 7); // Cài đặt độ lớn âm thanh
    }

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
    gWin = Mix_LoadWAV("Sound/Victory.wav");
    if (gWin == nullptr)
    {
        std::cout << "Failed to load gWin SOund! Error: " << Mix_GetError() << std::endl;
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
    SDL_Surface *gameOverSurface = IMG_Load("image/GameOver.png");
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
    if (Mix_PlayChannel(-1, gGameOver, 0) == -1)
    {
        std::cout << "Failed to play GameOver sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    SDL_RenderPresent(gRenderer);
}

void Game::renderWinMenu()
{
    SDL_Surface *gameOverSurface = IMG_Load("image/winmenu.png");
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
    if (Mix_PlayChannel(-1, gWin, 0) == -1)
    {
        std::cout << "Failed to play Win sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    SDL_RenderPresent(gRenderer);
}

void Game::loadExplosion() // render explosion
{
    if (isCollision())
    {
        if (!exp.isExploding()) // Kiểm tra vụ nổ có đang diễn ra không để tránh việc load ảnh và bắt đầu vụ nổ khi đang có vụ nổ khác
        {
            exp.loadExplosionImage(gRenderer, "image/exp_main.png");
            exp.start(gSquareRect.x, gSquareRect.y);
        }
    }
    for (auto &bulletB : Boss::getBulletBoss())
    {
        if (SDL_HasIntersection(&gSquareRect, &bulletB.rect))
        {
            if (!exp.isExploding())
            {
                exp.loadExplosionImage(gRenderer, "image/exp_main.png");
                exp.start(gSquareRect.x, gSquareRect.y);
            }
        }
    }

    if (exp.isExploding())
    {
        exp.update();
        exp.render(gRenderer);
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
