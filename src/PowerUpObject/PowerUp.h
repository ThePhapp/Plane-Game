#ifndef POWERUP_H
#define POWERUP_H

#include <SDL2/SDL.h>

enum PowerUpType
{
    POWERUP_SHIELD,      // Khiên bảo vệ
    POWERUP_RAPID_FIRE,  // Bắn nhanh
    POWERUP_DOUBLE_SHOT, // Bắn đôi
    POWERUP_HEALTH,      // Hồi máu
    POWERUP_BOMB         // Phá hủy tất cả enemies
};

class PowerUp
{
public:
    PowerUp();
    PowerUp(int x, int y, PowerUpType type);
    ~PowerUp();

    void update();
    void render(SDL_Renderer* renderer);
    void setTexture(SDL_Texture* value) { texture = value; }
    
    SDL_Rect getRect() const { return rect; }
    PowerUpType getType() const { return type; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    Uint32 getDuration() const { return duration; }

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    PowerUpType type;
    bool active;
    int speed;
    Uint32 duration; // Thời gian hiệu lực (ms)
};

#endif // POWERUP_H
