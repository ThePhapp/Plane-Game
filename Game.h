#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    bool initSDL();
    SDL_Texture* loadTexture(const std::string& path);
    void closeSDL();

    void render();
    void handleEvent(SDL_Event& e, bool& quit);

private:
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 800;
    const int SQUARE_SIZE = 60;

    SDL_Window* gWindow = nullptr;
    SDL_Renderer* gRenderer = nullptr;
    SDL_Texture* gSquareTexture = nullptr;
    SDL_Texture* gBulletTexture = nullptr;
    SDL_Rect gSquareRect;
    SDL_Rect gBulletRect;

    bool isBulletActive = false;
};

#endif // GAME_H
