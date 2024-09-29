<h1 align="center">
  <img src="image/Player.png" width="32px">
        War's Plane 
  
</h1>

### Welcome to "War's Plane: Sky Combat"! 🚀🌌

"War's Plane" is an exciting airplane shooting game, written in C++ and built using the SDL2 library. In this game, you'll experience the thrill of participating in intense aerial battles between different fighter planes.
#Video Demo: [Youtube.com](https://www.youtube.com/watch?v=eDS8nbxLrMA)

## 1.General Description <br />
### Game Objects

- <img src="image/Player.png" width="32px"> Player: The main character of the game. The player can move in four directions (up/down/left/right) controlled by the player.
- <img src="image/obstacle.png" width="32px"> Enemy (Obstacle): Objects that the player must destroy. Enemies spawn randomly and move at random speeds.
- <img src="image/Boss.png" width="32px"> Boss: The final opponent the player must defeat. The boss moves up and down and shoots bullets randomly.
- ![](image/Bullet.png)   ![](image/BossBullet.png) *Bullet* Objects fired by both the player and the boss. The player can shoot bullets continuously by pressing the SPACE key, while the boss fires bullets at random intervals. The boss's bullets always move from right to left.
- <img src="image/health.png" width="56px"> Lives: Represents the player's lives and score.

### How to Play
![image](https://github.com/ThePhapp/PRJ/assets/161786445/aa5ba866-4510-4b45-a3a5-71c86dfb4b57)

- Click "Play" to start the game.
- Move the player using the arrow keys ↑ ↓ ← → and fire bullets by pressing the SPACE key.
- In each round, the player will shoot down enemies, dodge bullets, and score 100 points for each enemy destroyed.
- The player starts with 3 lives <img src="image/health.png" width="56px"> and will lose one life when colliding with an enemy, getting hit by the boss's bullet, or missing 4 shots. The game ends when the player has no lives left.
- Enemies are destroyed when hit by the player's bullets.
- The boss loses health each time it's hit by the player's bullets and will be defeated once its health is depleted.

### Objectives
![image](https://github.com/ThePhapp/PRJ/assets/161786445/009d4b8e-bbb0-443d-8779-0832e258d74d)
- Shoot down enemies <img src="image/obstacle.png" width="32px"> to gain points.
- Defeat the boss <img src="image/Boss.png" width="32px"> to win the game.


### Main Game Components
- Game.cpp: The main function of the game, responsible for overall control.
- Bullet.cpp: Handles bullet creation for the player and processes related operations.
- BossObject.cpp: Creates the boss, handles movement, and boss bullet firing.
- ExplosionObject: Initializes and renders explosions when the player collides or the boss dies.
- Collision.cpp: Manages collision detection.
- Control.cpp: Handles game events like user input.
- ImageLoad.cpp: Loads images, sounds, and fonts.
 ## 2. Download the Game
- Click "Download" to get the .zip file: Download
- Unzip the file.
- Run the main.exe file to start the game.
 ## 3. Programming Techniques<br />
 
### Built on C++ and SDL2, including:
- SDL_Mixer: For sound processing.
- SDL_Ttf: For font processing.
- C++ libraries.
- Visual Studio Code IDE.

### Programming Skills Applied
- Basic C++ techniques (struct, class, vector, loops, etc.).
- Variables (static, extern), loops, branching structures, arrays, and strings.
- Collision detection techniques.
- Code management by splitting into different files.
- Use of SDL2, SDL2_Image, SDL2_TTF, and SDL2_Mixer.

### References
- [Lazyfoo](https://lazyfoo.net/tutorials/SDL/)
