#ifndef H_MAINMENU
#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#define H_MAINMENU
void MM_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void MM_mouseMove(GLFWwindow* window, double xpos, double ypos);
void MM_resizeWindow(GLFWwindow* window, int width, int height);

void InitMenu(void (*SetScreen)(int), int w, int h);
void UpdateMainMenu(float deltaTime);
void CleanUpMainMenu();
#endif
