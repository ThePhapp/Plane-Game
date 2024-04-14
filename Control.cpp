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
        gSquareRect.y -= 2;
        if (gSquareRect.y < 0)
        {
            gSquareRect.y = 0;
        }
    }

    if (currentKeyStates[SDL_SCANCODE_DOWN])
    {
        gSquareRect.y += 2;
        if (gSquareRect.y > SCREEN_HEIGHT - gSquareRect.w)
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
        if (gSquareRect.x > SCREEN_WIDTH - gSquareRect.w)
        {
            gSquareRect.x = SCREEN_WIDTH - gSquareRect.w;
        }
    }
}