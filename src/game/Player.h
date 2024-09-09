#include "../engine/primitives.h"
#include "input.h"
#include <stdbool.h>
#ifndef H_PLAYER
#define H_PLAYER
typedef enum PLAYER_ANIM_STATE {
  IDLE_DOWN = 0,
  IDLE_RIGHT,
  IDLE_UP,
  IDLE_LEFT,
  RUN_DOWN,
  RUN_RIGHT,
  RUN_UP,
  RUN_LEFT,
} PLAYER_ANIM_STATE;
typedef struct Player {
  float x;
  float y;
  float z;
  Sprite *sprite;
  Animation *anim;
  int state;
  int framerate;
  float frameTimer;
} Player;

void Animate(Player *player, Animation anim, bool loop, float deltaTime, unsigned int VBO);
void processPlayerMovementNew(Player *player, Inputs *input, float deltaTime, int tiles[1024]);
#endif
