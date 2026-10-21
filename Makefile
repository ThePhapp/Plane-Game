CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc/include -Isrc
LDFLAGS = -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SRC = \
    src/main.cpp \
    src/Game.cpp \
    src/Control.cpp \
    src/Resources.cpp \
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
DEP = $(OBJ:.o=.d)
TARGET = PlaneGame.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

.PHONY: all clean

ifeq ($(OS),Windows_NT)
clean:
	-del /Q $(subst /,\\,$(TARGET) $(OBJ) $(DEP))
else
clean:
	rm -f $(TARGET) $(OBJ) $(DEP)
endif
