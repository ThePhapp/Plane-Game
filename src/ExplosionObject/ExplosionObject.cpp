#include "explosionobject.h"
#include <SDL2/SDL_image.h>

#define EXP_WIDTH 64
#define EXP_HEIGHT 64
#define EXPLOSION_FRAMES 4
#define EXPLOSION_FRAME_DELAY 5

Explosion::Explosion() : exploding_(false), currentFrame_(0), frameDelay_(100), lastFrameTime_(0)
{
    explosionRect = {-EXP_WIDTH, -EXP_HEIGHT, 130, 130};
}

Explosion::~Explosion()
{
    SDL_DestroyTexture(explosionTexture);
}

void Explosion::update()
{
    if (exploding_)
    {
        int currentTime = SDL_GetTicks();

        if (currentTime - lastFrameTime_ >= frameDelay_)
        {
            currentFrame_++;
            lastFrameTime_ = currentTime;
        }

        if (currentFrame_ >= EXPLOSION_FRAMES)
        {
            exploding_ = false;
        }
    }
}

void Explosion::render(SDL_Renderer *renderer)
{
    if (exploding_ && explosionTexture)
    {
        SDL_Rect clip = clips[currentFrame_];
        SDL_RenderCopy(renderer, explosionTexture, &clip, &explosionRect);
        SDL_Delay(100);
    }
}
void Explosion::start(int x, int y)
{
    exploding_ = true;
    currentFrame_ = 0;
    explosionRect.x = x - EXP_WIDTH / 2;
    explosionRect.y = y - EXP_HEIGHT / 2;
    lastFrameTime_ = SDL_GetTicks(); // Reset frameDelayCounter_ khi bắt đầu vụ nổ mới
}

void Explosion::loadExplosionImage(SDL_Renderer *renderer, const char *filePath)
{
    SDL_Surface *surface = IMG_Load(filePath);
    if (surface)
    {
        explosionTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (explosionTexture == nullptr)
        {
            std::cout << "Failed to create texture from surface. SDL Error: " << SDL_GetError() << std::endl;
        }

        // Khởi tạo các clip animation
        for (int i = 0; i < EXPLOSION_FRAMES; i++)
        {
            SDL_Rect clip;
            clip.x = i * EXP_WIDTH;
            clip.y = 0;
            clip.w = EXP_WIDTH;
            clip.h = EXP_HEIGHT;
            clips.push_back(clip);
        }
    }
    else
    {
        std::cout << "Failed to load image. SDL_Image Error: " << IMG_GetError() << std::endl;
    }
}
