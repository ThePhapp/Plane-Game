#ifndef BOSSOBJECT_H
#define BOSSOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include "BulletObject/bullet.h"
#include "ExplosionObject/ExplosionObject.h"

class Boss
{
public:
    Boss();
    ~Boss();

    void render(SDL_Renderer *renderer);
    void update();
    bool handleBulletCollision(); // trả về true nếu boss chết
    void shootBullet();
    void loadBossImage(SDL_Renderer *renderer, const char *filePath);
    void loadBulletImage(SDL_Renderer *renderer, const char *filePath);

    SDL_Rect getRect() const { return rect; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    void setHealth(int healthB) { health = healthB; }
    void setBulletSpeed(int s) { bulletSpeed = s; }
    void setSpeed(int s) { speed = s; }
    bool isExploding() const { return exp1.isExploding(); }
    void startExplosion(SDL_Renderer* renderer);
    Uint32 getExplosionStartTime() const { return explosionStartTime; }
    void loadExplosionBoss(SDL_Renderer *renderer);

    static std::vector<Bullet> &getBulletBoss() { return bulletBoss; }

private:
    SDL_Texture *bossTexture;
    SDL_Texture *bulletTexture;
    SDL_Rect rect;
    bool active;
    int speed;
    int health;
    int bulletSpeed;
    Uint32 lastShootTime;
    Uint32 explosionStartTime;
    static std::vector<Bullet> bulletBoss;
    Explosion exp1;
};

#endif // BOSSOBJECT_H