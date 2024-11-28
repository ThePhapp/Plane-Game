#ifndef EXPLOSIONOBJECT_H
#define EXPLOSIONOBJECT_H

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

class Explosion
{
public:
    Explosion();
    ~Explosion();

    void update();
    void render(SDL_Renderer *renderer);
    void start(int x, int y); // Khởi tạo vị trí cho animation nổ
    void loadExplosionImage(SDL_Renderer *renderer, const char *filePath);
    bool isExploding() const
    {
        return exploding_;
    }

private:
    SDL_Rect explosionRect;
    SDL_Texture *explosionTexture = nullptr;
    std::vector<SDL_Rect> clips;
    bool exploding_;
    int currentFrame_;                       // Frame hiện tại của animation
    int frameDelay_;                         // Thời gian trễ giữa các frames (mili giây)
    int lastFrameTime_;                      // Thời gian khi frame cuối cùng được cập nhật

};

#endif // EXPLOSIONOBJECT_H
