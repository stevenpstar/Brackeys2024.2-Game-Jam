#include "../../dep/linmath.h"
#include "../game/input.h"
#include <stdbool.h>
#ifndef H_CAMERA
#define H_CAMERA
typedef struct Camera {
  vec3 position;
  vec3 target;
  vec3 direction;
  float yaw;
  float pitch;
  float roll;
  float speed;
  float sensitivity;
  double lastX;
  double lastY;
  mat4x4 proj;
  float fov;
} Camera;
float degToRad(float degree);
Camera createCamera(float *position, float *target, float speed);
void mouseLook(float xoff, float yoff, Camera *camera, float deltaTime);
void setDirection(Camera *camera);
void setProjection(unsigned int shader, const char* uniformName, Camera *camera, bool perspective, float w, float h);
void setFOV(float fov, Camera *camera);
void processCameraMovement(Camera *camera, bool *idle, Inputs *inputs, bool fpsMode, vec3 up);
#endif
