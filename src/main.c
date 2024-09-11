#include "../dep/glad/glad.h"
#include "../dep/GLFW/glfw3.h"
#include "../dep/linmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "game/game.h"
#include "game/songeditor.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
vec3 lightPos = {0.0f, -2.0f, 0.0f};

typedef enum SCREEN {
  MAINMENU,
  OPTIONS,
  GAME,
  SONGEDITOR,
} SCREEN;

GLFWwindow *window;
SCREEN currentScreen;

void SetScreen(int screen);
void SetGameScreen();
void SetSongEditorScreen();

int main(void) {
  srand(time(NULL));
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Base", NULL, NULL);
  if (window == NULL) {
    printf("GLFW Window creation failed\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialise glad");
    glfwTerminate();
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glViewport(0, 0, 800, 600);

 // glfwSetFramebufferSizeCallback(window, resizeWindow);
//  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//  glfwSetCursorPosCallback(window, mouseMove);
//  glfwSetKeyCallback(window, keyCallback);

//  InitGame();
//  SetupLighting();
  SetScreen(GAME);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (currentScreen == GAME) {
      GameUpdate(deltaTime);
    } else if (currentScreen == SONGEDITOR) {
      UpdateSongEditor(deltaTime);
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  if (currentScreen == GAME) {
    DeleteBuffers();
  } else if (currentScreen == SONGEDITOR) {
    CleanUpSongEditor();
  }
  glfwTerminate();
  return 0;
}

void SetScreen(int screen) {
  switch (screen) {
    case MAINMENU:
      break;
    case OPTIONS:
      break;
    case GAME:
      SetGameScreen();
      break;
    case SONGEDITOR:
      SetSongEditorScreen();
      break;
    default:
      break;
  }
}

void SetGameScreen() {
  InitGame();
  SetupLighting();
  currentScreen = GAME;
  glfwSetFramebufferSizeCallback(window, resizeWindow);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseMove);
  glfwSetKeyCallback(window, keyCallback);
}

void SetSongEditorScreen() {
  InitSongEditor();
  currentScreen = SONGEDITOR;
  glfwSetFramebufferSizeCallback(window, SE_resizeWindow);
  glfwSetCursorPosCallback(window, SE_mouseMove);
  glfwSetKeyCallback(window, SE_keyCallback);

}
