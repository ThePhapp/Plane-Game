#include <SDL2/SDL.h>
#include <iostream>

// Kích thước cửa sổ
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    bool isRunning = true;

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo cửa sổ
    window = SDL_CreateWindow("Simple Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Tạo renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Vị trí ban đầu của nhân vật
    int x = WINDOW_WIDTH / 2;
    int y = WINDOW_HEIGHT / 2;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            // Xử lý sự kiện nhấn phím
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        y -= 10;
                        break;
                    case SDLK_DOWN:
                        y += 10;
                        break;
                    case SDLK_LEFT:
                        x -= 10;
                        break;
                    case SDLK_RIGHT:
                        x += 10;
                        break;
                }
            }
        }

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ nhân vật
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = { x, y, 50, 50 };
        SDL_RenderFillRect(renderer, &rect);

        // Update màn hình
        SDL_RenderPresent(renderer);
    }

    // Giải phóng bộ nhớ
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
