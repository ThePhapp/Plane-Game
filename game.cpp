#include "Game.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_SIZE = 40;

Game::Game() : gWindow(nullptr), gRenderer(nullptr), gPlayerSpeed(1), gIsRunning(true) {
    initializeSDL();
    gPlayerRect = { WINDOW_WIDTH / 2 - PLAYER_SIZE / 2, WINDOW_HEIGHT / 2 - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE };
}

Game::~Game() {
    cleanupSDL();
}

void Game::initializeSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}

void Game::cleanupSDL() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            gIsRunning = false;
            break;
        }
    }

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
        gIsRunning = false;
    }

    if (currentKeyStates[SDL_SCANCODE_W]) {
        gPlayerRect.y -= gPlayerSpeed;
    }
    if (currentKeyStates[SDL_SCANCODE_S]) {
        gPlayerRect.y += gPlayerSpeed;
    }
    if (currentKeyStates[SDL_SCANCODE_A]) {
        gPlayerRect.x -= gPlayerSpeed;
    }
    if (currentKeyStates[SDL_SCANCODE_D]) {
        gPlayerRect.x += gPlayerSpeed;
    }
}

void Game::update() {
    // Update game state here
}

void Game::render() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderFillRect(gRenderer, &gPlayerRect);

    SDL_RenderPresent(gRenderer);
}

void Game::run() {
    while (gIsRunning) {
        handleEvents();
        update();
        render();
    }
}
