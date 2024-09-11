#ifndef H_SONGEDITOR
#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#include "../../dep/linmath.h"
#define H_SONGEDITOR
typedef struct SESquare {
  vec2 position;
  float dim;
  vec3 colour;
} SESquare;
void SE_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void SE_mouseMove(GLFWwindow* window, double xpos, double ypos);
void SE_resizeWindow(GLFWwindow* window, int width, int height);

void InitSongEditor();
void UpdateSongEditor(float deltaTime);
void RenderGrid();
void CleanUpSongEditor();
#endif
