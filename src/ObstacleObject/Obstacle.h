#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL2/SDL.h>

enum EnemyType
{
    ENEMY_BASIC,
    ENEMY_FAST,      // Di chuyển nhanh
    ENEMY_TANK,      // Nhiều máu hơn
    ENEMY_KAMIKAZE   // Lao thẳng vào player
};

class Obstacle
{
public:
    Obstacle();
    Obstacle(int x, int y, EnemyType type);
    ~Obstacle();

    void update(SDL_Rect playerRect, float deltaSeconds);
    void render(SDL_Renderer* renderer);
    void setTexture(SDL_Texture* value) { texture = value; }
    bool takeDamage(int damage);
    
    SDL_Rect getRect() const { return rect; }
    EnemyType getType() const { return type; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    int getHealth() const { return health; }
    int getPoints() const { return points; }

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    EnemyType type;
    bool active;
    int speed;
    int health;
    int points; // Điểm khi tiêu diệt
    int maxHealth;
};

#endif // OBSTACLE_H
