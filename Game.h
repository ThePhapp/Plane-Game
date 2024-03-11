#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void initializeSDL();
    void cleanupSDL();
    void handleEvents();
    void update();
    void render();

    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    SDL_Rect gPlayerRect;
    int gPlayerSpeed;
    bool gIsRunning;
};

#endif // GAME_H
