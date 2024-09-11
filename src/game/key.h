#include "../engine/primitives.h"
#include <stdbool.h>
#ifndef H_KEY
#define H_KEY
void renderKey(
    UISprite *key,
    bool flag,
    unsigned int VBO,
    unsigned int texture,
    unsigned int shader,
    int windowWidth,
    int windowHeight);
#endif
