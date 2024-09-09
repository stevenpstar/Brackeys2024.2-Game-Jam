#include "Player.h"
#include "tile.h"
#include "../engine/collision.h"
#include "../../dep/linmath.h"
#include <stdbool.h>

void Animate(Player *player, Animation anim, bool loop, float deltaTime, unsigned int VBO) {
  int startFrame = anim.startFrame;
  int endFrame = anim.endFrame;
  float frameTime = player->framerate / 60.0f;
  bool nextFrame = false;
  bool flipHoriz = false;
  // This is stupid but ye
  if (player->anim->state == RUN_LEFT || player->anim->state == IDLE_LEFT) {
    flipHoriz = true;
    //printf("FLIP!");
  }
  if (player->sprite->currentFrame < startFrame || player->sprite->currentFrame > endFrame) {
    player->sprite->currentFrame = startFrame;
    player->frameTimer = 0.0f;
    SetFrame(player->sprite, player->sprite->currentFrame, VBO, flipHoriz);
  }
  player->frameTimer += deltaTime;
  if (player->frameTimer >= frameTime) {
    nextFrame = true;
    player->frameTimer -= frameTime;
  }
  if (!nextFrame) {
    return;
  }
  if (player->sprite->currentFrame == endFrame) {
    player->sprite->currentFrame = startFrame;
  } else {
    player->sprite->currentFrame++;
  }

  SetFrame(player->sprite, player->sprite->currentFrame, VBO, flipHoriz);
}

void processPlayerMovementNew(Player *player, Inputs *input, float deltaTime, int *tiles) {
  float playerSpeed = 2.f;
  float diagonalMod = 0.70710678118;

  Bounds boundsA = {.x = 0.0f, .y = 0.0f, .width = 0.0f, .height = 0.0f};
  Bounds boundsB = {.x = 0.0f, .y = 0.0f, .width = 0.0f, .height = 0.0f};

  bool forwardDown = input->forward.Down;
  bool backDown = input->backwards.Down;
  bool leftDown = input->left.Down;
  bool rightDown = input->right.Down;

  int mVert = 0;
  int mHoriz = 0;

  if (forwardDown && rightDown || forwardDown && leftDown ||
      backDown && rightDown || backDown && leftDown) {
    playerSpeed *= diagonalMod;
  }
  if (forwardDown) {
    if (player->anim->state != RUN_UP) {
      player->anim->state = RUN_UP;
      player->anim->startFrame = 20;
      player->anim->endFrame = 23;
      printf("player->anim->changing?: %d\n", player->anim->state);
    }
    mVert = -1;
  }
  if (backDown) {
    if (player->anim->state != RUN_DOWN) {
      player->anim->state = RUN_DOWN;
      player->anim->startFrame = 12;
      player->anim->endFrame = 15;
    }
    mVert = 1;
  }
  if (leftDown) {
    if (player->anim->state != RUN_LEFT) {
      player->anim->state = RUN_LEFT;
      player->anim->startFrame = 16;
      player->anim->endFrame = 19;
    }
    mHoriz = -1;
  }
  if (rightDown) {
  if (player->anim->state != RUN_RIGHT) {
      player->anim->state = RUN_RIGHT;
      player->anim->startFrame = 16;
      player->anim->endFrame = 19;
    }
    mHoriz = 1;
  }
  if (!forwardDown && !backDown && !leftDown && !rightDown) {
    if (player->anim->state == RUN_UP) {
      player->anim->state = IDLE_UP;
      player->anim->startFrame = 8;
      player->anim->endFrame = 9;
    }
    else if (player->anim->state == RUN_RIGHT) {
      player->anim->state = IDLE_RIGHT;
      player->anim->startFrame = 4;
      player->anim->endFrame = 5;
    }
    else if (player->anim->state == RUN_LEFT) {
      player->anim->state = IDLE_LEFT;
      player->anim->startFrame = 4;
      player->anim->endFrame = 5;
    }
    else if (player->anim->state == RUN_DOWN) {
      player->anim->state = IDLE_DOWN;
      player->anim->startFrame = 0;
      player->anim->endFrame = 1;
    }
  }
  vec2 nextPosition = {0};
  nextPosition[0] = player->x + mHoriz * playerSpeed * deltaTime;
  nextPosition[1] = player->y + mVert * playerSpeed * deltaTime;
  boundsA.width = 0.5f;
  boundsA.height = 0.5f;
  boundsA.x = nextPosition[0];
  boundsA.y = nextPosition[1];
  int nextTile[2] = {0};
  getTileFromPosition(nextPosition[0], nextPosition[1], nextTile);
  int tileIndex = getIndexFromTile(nextTile[0], nextTile[1], 32);
  bool checkCollide = tiles[tileIndex] == 34;

  boundsB.width = 0.5f;
  boundsB.height = 0.5f;
  boundsB.x = nextTile[0] * 0.5f;
  boundsB.y = nextTile[1] * 0.5f;

  if (!IsColliding(&boundsA, &boundsB) || !checkCollide) {
    player->x += mHoriz * playerSpeed * deltaTime;
    player->z += mVert * playerSpeed * deltaTime;
  }
}
