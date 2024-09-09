#include "../../dep/glad/glad.h"
#include "../../dep/linmath.h"
#ifndef H_TILE
#define H_TILE
// we will do things with this later ;)
typedef struct Tile {
} Tile;

typedef struct TileXY {
  int x;
  int y;
} TileXY;

unsigned int createWorld(int tiles[1024], const char* tilesFile, GLubyte pixelData[32*32*3]);
void setTileData(int i, int tileSize, int texWidth, int texHeight, GLubyte pixelData[32*32*3], float data[30], int unsigned VBO);
void getTileFromPosition(float x, float y, int *pos);
void resetTileTexCoords(float data[48], unsigned int VBO);
void renderTile(int tile, int row, int col, mat4x4 model, mat4x4 view, unsigned int texture, unsigned int VBO, unsigned int shader);
void renderWall(int tile, int row, int col, mat4x4 model, mat4x4 view, unsigned int texture, unsigned int VBO, unsigned int shader);
int getIndexFromTile(int x, int y, int mapWidth);
#endif
