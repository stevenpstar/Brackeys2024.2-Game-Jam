#include "../dep/glad/glad.h"
#include "../dep/GLFW/glfw3.h"
#include "../dep/linmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "game/game.h"
#include "game/selectsong.h"
#include "game/songeditor.h"
#include "game/mainmenu.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
vec3 lightPos = {0.0f, -2.0f, 0.0f};
char selectedSongFile[512];
bool initialLoad = true;

typedef enum SCREEN {
  MAINMENU,
  OPTIONS,
  GAME,
  SONGSELECT,
  SONGEDITOR,
} SCREEN;

GLFWwindow *window;
SCREEN currentScreen;

void SetScreen(int screen);
void SetSelectedSong(char song[512]);
void SetGameScreen();
void SetSongSelectScreen();
void SetSongEditorScreen();
void SetMainMenuScreen();

int main(void) {
  strcpy(selectedSongFile, "song1");
  srand(time(NULL));
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(
           glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
           glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
           "A Knight's Rest", glfwGetPrimaryMonitor(), NULL);
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

  SetScreen(GAME);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (currentScreen) {
      case GAME:
        GameUpdate(deltaTime);
        break;
      case MAINMENU:
        UpdateMainMenu(deltaTime);
        break;
      case SONGSELECT:
        UpdateSelectSong(deltaTime);
        break;
      case SONGEDITOR:
        UpdateSongEditor(deltaTime);
        break;
      case OPTIONS:
        //not implemented yet
        break;
      default:
        // we shouldn't get here
        UpdateMainMenu(deltaTime);
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  switch (currentScreen) {
    case GAME:
      DeleteBuffers();
      break;
    case MAINMENU:
      CleanUpMainMenu();
      break;
    case SONGSELECT:
      CleanUpSelectSong();
      break;
    case SONGEDITOR:
      CleanUpSongEditor();
      break;
    case OPTIONS:
      //not implemented yet
      break;
    default:
      // we shouldn't get here
      CleanUpMainMenu();
  }
  glfwTerminate();
  return 0;
}

void SetScreen(int screen) {
  if (!initialLoad) {
    if (screen == MAINMENU) {
      printf("Might crash here on initial load\n");
      CleanUpMainMenu();
    } else if (screen == SONGSELECT) {
      CleanUpSelectSong();
    } else if (screen == GAME) {
      DeleteBuffers();
    } 
  } else {
    initialLoad = false;
  }
  switch (screen) {
    case MAINMENU:
      SetMainMenuScreen();
      break;
    case OPTIONS:
      break;
    case GAME:
      SetGameScreen();
      break;
    case SONGSELECT:
      SetSongSelectScreen();
      break;
    case SONGEDITOR:
      SetSongEditorScreen();
      break;
    default:
      break;
  }
}

void SetGameScreen() {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  InitGame(SetScreen, selectedSongFile, width, height);
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

void SetMainMenuScreen() {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  InitMenu(SetScreen, width, height);
  currentScreen = MAINMENU;
  glfwSetFramebufferSizeCallback(window, MM_resizeWindow);
//  glfwSetCursorPosCallback(window, SE_mouseMove);
  glfwSetKeyCallback(window, MM_keyCallback);
}

void SetSongSelectScreen() {
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  InitSelectSong(SetScreen, SetSelectedSong, width, height);
  currentScreen = SONGSELECT;
  glfwSetFramebufferSizeCallback(window, SM_resizeWindow);
//  glfwSetCursorPosCallback(window, SE_mouseMove);
  glfwSetKeyCallback(window, SM_keyCallback);

}

void SetSelectedSong(char song[512]) {
  // TODO: spooky code
  strcpy(selectedSongFile, song);
}
