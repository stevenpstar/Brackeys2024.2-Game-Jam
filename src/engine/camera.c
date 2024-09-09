#include "camera.h"
#include "../../dep/linmath.h"
#include "../../dep/glad/glad.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

float degToRad(float degree) {
  return degree * M_PI / 180;
}

Camera createCamera(float *position, float *target, float speed) {
  Camera cam = {
    .position = {position[0], position[1], position[2]},
    .target = {target[0], target[1], target[2]},
    .speed = speed,
    .yaw = -90.0f,
    .pitch = 0.0f,
    .roll = 0.0f,
    .sensitivity = 0.04f,
    .lastX = 400.0,
    .lastY = 300.0,
    .fov = 0.9f,
  };

  mat4x4_identity(cam.proj);
  mat4x4_perspective(cam.proj, cam.fov, 800.0f / 600.0f, 0.1f, 100.0f);
  //mat4x4_ortho(cam.proj, 0.0f, 800.0f, 0.0f, 600.0f, 0.01f, 10000.0f);

  cam.direction[0] = cos(degToRad(cam.yaw));
  cam.direction[1] = sin(degToRad(cam.pitch));
  cam.direction[2] = sin(degToRad(cam.yaw));
  vec3_norm(cam.direction, cam.direction);

  return cam;
}

void setDirection(Camera *camera) {
  vec3 lookAhead;
  lookAhead[0] = cos(degToRad(camera->yaw));
  lookAhead[1] = sin(degToRad(camera->pitch));
  lookAhead[2] = sin(degToRad(camera->yaw));
  vec3_norm(camera->direction, lookAhead);
}

void setProjection(unsigned int shader, const char* uniformName, Camera *camera, bool perspective, float w, float h) {
  mat4x4_identity(camera->proj);
  if (perspective) {
    mat4x4_perspective(camera->proj, camera->fov, (float)w / (float)h, 0.1f, 100.0f);
  } else {
    printf("Setting ortho!");
    mat4x4_ortho(camera->proj, 0.0f, 800.0f, 0.0f, 600.0f, 0.01f, 10000.0f);
  }

  unsigned int projLoc = glGetUniformLocation(shader, uniformName);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, (GLfloat *)camera->proj);
}

void mouseLook(float xoff, float yoff, Camera *camera, float deltaTime) {
  xoff *= camera->sensitivity;// * deltaTime;
  yoff *= camera->sensitivity;// * deltaTime;
  camera->yaw += xoff;
  camera->pitch += yoff;
  if (camera->pitch > 89.0f) {
    camera->pitch = 89.0f;
  } else if (camera->pitch < -89.0f) {
    camera->pitch = -89.0f;
  }
  setDirection(camera);
}

void setFOV(float fov, Camera *camera) {
  //camera->fov = fov;
  //mat4x4_perspective(camera->proj, camera->fov, 800.0f / 600.0f, 0.1f, 100.0f);
}

void processCameraMovement(Camera *camera, Inputs *inputs, bool fpsMode, vec3 up) {
  vec3 camFront;
  if (fpsMode) {
    camFront[0] = 0.0f;
    camFront[1] = 0.0f;
    camFront[2] = -1.0f;
  } else {
    camFront[0] = 0.0f;
    camFront[1] = -1.0f;
    camFront[2] = 0.0f;
  }
  vec3 nFront;
  vec3 pos = {camera->position[0], camera->position[1], camera->position[2]};
//  vec3 pos = {camera->position[0], camera->position[1], camera->position[2]};
  if (inputs->forward.Down) {
    vec3_scale(nFront, camera->direction, 0.025f);
    vec3_add(camera->position, pos, nFront);
  }
  if (inputs->backwards.Down) {
    vec3_scale(nFront, camera->direction, -0.025f);
    vec3_add(camera->position, pos, nFront);
  }
  if (inputs->left.Down) {
    vec3 cross;
    vec3_mul_cross(cross, camera->direction, up);
    vec3_norm(cross, cross);
    vec3_scale(cross, cross, -0.0125f);
    vec3_add(camera->position, camera->position, cross);
  }
  if (inputs->right.Down) {
    vec3 cross;
    vec3_mul_cross(cross, camera->direction, up);
    vec3_norm(cross, cross);
    vec3_scale(cross, cross, 0.0125f);
    vec3_add(camera->position, camera->position, cross);
  }
}


