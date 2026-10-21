#ifndef BOSSOBJECT_H
#define BOSSOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include "BulletObject/Bullet.h"
#include "ExplosionObject/ExplosionObject.h"

enum BossType
{
    BOSS_NORMAL,
    BOSS_FAST,       // Nhanh, bắn nhiều
    BOSS_TANK,       // Chậm, máu nhiều
    BOSS_ULTIMATE    // Boss cuối, pattern phức tạp
};

class Boss
{
public:
    Boss();
    Boss(BossType type);
    ~Boss();

    void render(SDL_Renderer *renderer);
    void update(float deltaSeconds);
    bool handleBulletCollision();
    void shootBullet();
    void shootPattern(); // Different shooting patterns
    void loadBossImage(SDL_Renderer *renderer, const char *filePath);
    void loadBulletImage(SDL_Renderer *renderer, const char *filePath);

    SDL_Rect getRect() const { return rect; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    void setHealth(int healthB) { health = healthB; maxHealth = healthB; }
    void setBulletSpeed(int s) { bulletSpeed = s; }
    void setSpeed(int s) { speed = s; }
    void setType(BossType t) { type = t; }
    BossType getType() const { return type; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    bool isExploding() const { return exp1.isExploding(); }
    void startExplosion(SDL_Renderer* renderer);
    void reset();
    void releaseResources();
    Uint32 getExplosionStartTime() const { return explosionStartTime; }

    static std::vector<Bullet> &getBulletBoss() { return bulletBoss; }

private:
    SDL_Texture *bossTexture;
    SDL_Texture *bulletTexture;
    SDL_Rect rect;
    bool active;
    int speed;
    int health;
    int maxHealth;
    int bulletSpeed;
    BossType type;
    int phase; // Boss phase (for pattern changes)
    Uint32 lastShootTime;
    Uint32 explosionStartTime;
    static std::vector<Bullet> bulletBoss;
    Explosion exp1;
};

#endif // BOSSOBJECT_H
