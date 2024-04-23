#ifndef BOSSOBJECT_H
#define BOSSOBJECT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include "bullet.h"
#include "ExplosionObject.h"

class Boss
{
public:
    Boss();
    ~Boss();
    void render(SDL_Renderer *renderer);
    void update();
    void handleBulletCollision();
    void shootBullet();
    void loadBossImage(SDL_Renderer *renderer, const char *filePath);
    void loadBulletImage(SDL_Renderer *renderer, const char *filePath);
    SDL_Rect getRect() const { return rect; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    void setHealth(int healthB) { health = healthB; }
    static std::vector<Bullet> &getBulletBoss() { return bulletBoss; }
    void loadExplosionBoss(SDL_Renderer *renderer);
    Uint32 explosionStartTime;

private:
    SDL_Texture *bossTexture;
    SDL_Texture *bulletTexture;
    SDL_Rect rect;
    bool active;
    bool activeBullet;
    int speed;
    int health;
    int bulletSpeed;
    static std::vector<Bullet> bulletBoss;
    Explosion exp1;
};

#endif // BOSSOBJECT_H