#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
    void closeSDL();
    bool isBulletActive = false;
    bool isObstacleActive = false;
    void spawnObstacle();
    void updateObstacle();
    void handleBulletCollision();
    void render();
    void handleEvent(SDL_Event& e, bool& quit);
    void loadImagee();
    void loadBackground();
    void loadTextTexture(const std::string& text);
    void loadSoundd();
    bool isCollision();
    void renderPoints();
    void renderGameOver();
    void renderHealth();
private:
    const int SCREEN_WIDTH = 1300;
    const int SCREEN_HEIGHT = 900;
    const int SQUARE_SIZE = 80;
    int points;
    int highestPoint = 0;
    int maxHealth = 3;
    int currentHealth;
    const int HEALTH_WIDTH = 50;
    const int HEALTH_HEIGHT = 50;
    const int HEALTH_SPACING = 10;

    SDL_Window* gWindow = nullptr;
    SDL_Renderer* gRenderer = nullptr;
    SDL_Texture* gSquareTexture = nullptr;
    SDL_Texture* gBulletTexture = nullptr;
    SDL_Texture* gBackgroundTexture;
    SDL_Texture* textTexture;
    SDL_Texture* gObstacleTexture = nullptr;
    SDL_Texture* loadTexture(const std::string& path);
    SDL_Rect gSquareRect;
    SDL_Rect gBulletRect;
    SDL_Rect dstRect = {10, 10, 200, 50};
    SDL_Rect gObstacleRect;
    SDL_Texture* gHealth = nullptr; 
    Mix_Chunk* gShootSound = nullptr;
    Mix_Chunk* gCollision = nullptr;
    Mix_Chunk* gSound = nullptr;
    Mix_Chunk* gShotTrung = nullptr;
    Mix_Chunk* gGameOver = nullptr;


};

#endif // GAME_H
