#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#include "input.h"
void setDefaults(Inputs *inputs) {
  inputs->forward.Key = GLFW_KEY_W;
  inputs->forward.Down = false;

  inputs->backwards.Key = GLFW_KEY_S;
  inputs->backwards.Down = false;

  inputs->left.Key = GLFW_KEY_A;
  inputs->left.Down = false;

  inputs->right.Key = GLFW_KEY_D;
  inputs->right.Down = false;
}
