all:
g++ -Isrc/include -Lsrc/lib -o main main.cpp game.cpp collision.cpp bullet.cpp obstacle.cpp ImageLoad.cpp control.cpp bossobject.cpp explosionobject.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf 
 