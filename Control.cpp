#include "game.h"

void Game::handleEvent(SDL_Event &e, bool &quit)
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
                fireBullet();
                if (Mix_PlayChannel(-1, gShootSound, 0) == -1)
                {
                    std::cout << "Failed to play shoot sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }
            }
        }
    }

    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_ESCAPE])
    {
        quit = true;
    }

    if (currentKeyStates[SDL_SCANCODE_UP])
    {
        gSquareRect.y -= 7;
        if (gSquareRect.y < 0)
        {
            gSquareRect.y = 0;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        gSquareRect.y += 7;
        if (gSquareRect.y > SCREEN_HEIGHT - gSquareRect.w)
        {
            gSquareRect.y = SCREEN_HEIGHT - gSquareRect.w;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_LEFT])
    {
        gSquareRect.x -= 7;
        if (gSquareRect.x < 0)
        {
            gSquareRect.x = 0;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        gSquareRect.x += 7;
        if (gSquareRect.x > SCREEN_WIDTH - gSquareRect.w)
        {
            gSquareRect.x = SCREEN_WIDTH - gSquareRect.w;
        }
    }
}

void Game::handleMainMenuEvent(SDL_Event &e, bool &quit, bool &showMainMenu) // Main Menu
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
                miss = 0;
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