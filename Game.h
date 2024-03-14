#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "Collision.h"
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
    bool isBulletActive = false;
    bool isObstacleActive = false;
    SDL_Texture* gObstacleTexture = nullptr;
    SDL_Rect gObstacleRect;
    void spawnObstacle();
    void updateObstacle();
    void handleBulletCollision();
    void render();
    void handleEvent(SDL_Event& e, bool& quit);
    void loadBackground();
    void loadTextTexture(const std::string& text);
    bool isCollision();
private:
    const int SCREEN_WIDTH = 1300;
    const int SCREEN_HEIGHT = 900;
    const int SQUARE_SIZE = 60;

    SDL_Window* gWindow = nullptr;
    SDL_Renderer* gRenderer = nullptr;
    SDL_Texture* gSquareTexture = nullptr;
    SDL_Texture* gBulletTexture = nullptr;
    SDL_Texture* gBackgroundTexture;
    SDL_Texture* textTexture;
    SDL_Rect gSquareRect;
    SDL_Rect gBulletRect;
    SDL_Rect dstRect = {10, 10, 200, 50};


};

#endif // GAME_H
