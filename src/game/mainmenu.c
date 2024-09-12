#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#include <stdbool.h>
#include <stdio.h>
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "../../dep/gltext.h"
// game
#include "mainmenu.h"

int MM_windowWidth;
int MM_windowHeight;

GLTtext *MM_text;
int selectedButton = 0;
void (*MMSetScreen)(int);

void MM_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    selectedButton = 1;
  }
  if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    selectedButton = 0;
  }
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS || key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    if (selectedButton == 0) {
      MMSetScreen(3);
    } else if (selectedButton == 1) {
      glfwSetWindowShouldClose(window, true);
    }
  }
}
void MM_mouseMove(GLFWwindow* window, double xpos, double ypos) {}
void MM_resizeWindow(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  gltViewport(width, height);
}

void InitMenu(void (*SetScreen)(int), int w, int h) {
  gltInit();
  MM_text = gltCreateText();
  MMSetScreen = SetScreen;
  glViewport(0, 0, w, h);
  gltViewport(w, h);
  MM_windowWidth = w;
  MM_windowHeight = h;
}

void UpdateMainMenu(float deltaTime) {
    gltBeginDraw();

    gltSetText(MM_text, "Play");
    if (selectedButton == 0) {
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
      gltColor(0.8f, 0.8f, 0.8f, 1.0f);
    }
    gltDrawText2D(MM_text, 120.f, 120.f, 4.0f);

    gltSetText(MM_text, "Exit");
    if (selectedButton == 1) {
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
      gltColor(0.8f, 0.8f, 0.8f, 1.0f);
    }
    gltDrawText2D(MM_text, 120.0f, 240.0f, 4.0f);

    gltEndDraw();
}

void CleanUpMainMenu() {
  gltDeleteText(MM_text);
  gltTerminate();
  printf("Has we cleaned?\n");
}

