<h1 align="center">
  <img src="image/Player.png" width="32px">
        War's Plane: Sky Combat
</h1>

### Welcome to "War's Plane: Sky Combat"! 🚀🌌

"War's Plane" is an exciting airplane shooting game written in C++ using the SDL2 library. Experience intense aerial battles, defeat enemies, and challenge the boss with unique skills and bullet patterns!

[▶️ Video Demo](https://www.youtube.com/watch?v=eDS8nbxLrMA)

---

## 1. General Description

### Game Objects

- <img src="image/Player.png" width="32px"> **Player:** Move with arrow keys (↑ ↓ ← →), shoot with SPACE. Dodge enemy and boss bullets!
- <img src="image/obstacle.png" width="32px"> **Enemy (Obstacle):** Randomly spawned, destroyed by your bullets.
- <img src="image/Boss.png" width="32px"> **Boss:** The final challenge! Moves vertically, fires bullets, and has a health bar.
- ![](image/Bullet.png) ![](image/BossBullet.png) **Bullets:** Both player and boss fire bullets. Player bullets go right, boss bullets go left.
- <img src="image/health.png" width="56px"> **Lives:** You start with 3 lives. Lose one if hit by an enemy, boss bullet, or after missing 4 shots.

### Level Selection & Difficulty

- After clicking **Play**, you can choose your level:
  - **Easy:** Slower enemies and boss, less boss health.
  - **Medium:** Balanced challenge.
  - **Hard:** Fast enemies, boss fires rapidly, more health.
- Each level changes enemy speed, boss speed, and boss bullet speed for a unique experience.

### How to Play

- Click **Play** to start, then select your level.
- Move with arrow keys, shoot with SPACE.
- Destroy enemies for +100 points each.
- Survive and defeat the boss to win!
- Game ends when you lose all lives.

---

## 2. Main Game Components

- **Game.cpp:** Main game loop and logic.
- **Control.cpp:** Handles user input and game events.
- **Bullet.cpp:** Manages player bullets and collisions.
- **BossObject.cpp:** Boss logic, movement, and boss bullets.
- **ObstacleObject.cpp:** Enemy (obstacle) logic.
- **ExplosionObject.cpp:** Handles explosion effects.
- **Collision.cpp:** Collision detection.
- **ImageLoad.cpp:** Loads images, sounds, and fonts.

---

## 3. Download & Run

- Download the `.zip` file and extract it.
- Run `PlaneGame.exe` to start the game.
- Make sure you have all required DLLs (SDL2, SDL2_image, SDL2_ttf, SDL2_mixer) in the same folder.

---

## 4. Programming Techniques

- **C++ OOP:** Classes, structs, vectors, encapsulation.
- **SDL2:** Graphics, sound, fonts, and input handling.
- **Game Loop:** Real-time update and render.
- **Collision Detection:** For bullets, enemies, and boss.
- **Resource Management:** Efficient loading and freeing of textures, sounds, and fonts.
- **Modular Code:** Each game component in its own file.

---

## 5. References

- [Lazyfoo SDL2 Tutorials](https://lazyfoo.net/tutorials/SDL/)
- [SDL2 Documentation](https://wiki.libsdl.org/)

---

Enjoy the game and challenge yourself at every level!
