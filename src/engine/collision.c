#include "collision.h"
#include <stdio.h>
bool IsColliding(Bounds *a, Bounds *b) {
  return (a->x >= b->x && a->x < b->x + b->width &&
      a->y >= b->y && a->y < b->y + b->height);
}

void printBounds(Bounds *a) {
  printf("x: %f, y: %f, width: %f, height: %f\n", a->x, a->y, a->width, a->height);
}
