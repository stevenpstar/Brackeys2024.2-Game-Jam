#include <stdbool.h>
#include <stdio.h>
#ifndef H_COLLISION
#define H_COLLISION
typedef struct Bounds {
  float x;
  float y;
  float width;
  float height;
} Bounds;
bool IsColliding(Bounds *a, Bounds *b);
void printBounds(Bounds *a);
#endif
