CXX = g++
CXXFLAGS = -std=c++17 -Isrc/include -Isrc
LDFLAGS = -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SRC = \
    src/main.cpp \
    src/Game.cpp \
    src/Control.cpp \
    src/ImageLoad.cpp \
    src/Collision.cpp \
    src/BulletObject/Bullet.cpp \
    src/ObstacleObject/Obstacle.cpp \
    src/ExplosionObject/ExplosionObject.cpp \
    src/BossObject/BossObject.cpp \
    src/PowerUpObject/PowerUp.cpp \
    src/WeaponObject/Weapon.cpp \
    src/SaveSystem/SaveGame.cpp \
    src/AchievementSystem/Achievement.cpp \
    src/LeaderboardSystem/Leaderboard.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = PlaneGame.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	del /Q $(TARGET) src\*.o src\BulletObject\*.o src\ObstacleObject\*.o src\ExplosionObject\*.o src\BossObject\*.o src\PowerUpObject\*.o src\WeaponObject\*.o src\SaveSystem\*.o src\AchievementSystem\*.o src\LeaderboardSystem\*.o

#mingw32-make      