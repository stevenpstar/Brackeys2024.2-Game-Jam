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

float textAlpha1 = 0.0f;
float textAlpha2 = 0.0f;
bool startText1 = true;
bool startText2 = false;
int textStage = 1;

void MM_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    selectedButton = 1;
  }
  if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    selectedButton = 0;
  }
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS || key == GLFW_KEY_SPACE && action == GLFW_PRESS
      || key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//      MMSetScreen(3);
      textStage++;
      if (textStage == 2) {
        textAlpha1 = 1.0f;
        startText2 = true;
      } else if (textStage == 3) {
        textAlpha2 = 1.0f;
        MMSetScreen(2);
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

    gltSetText(MM_text, "Our heroes have made it to the final level of the dungeon...");
    gltColor(1.0f, 1.0f, 1.0f, textAlpha1);
    gltDrawText2DAligned(MM_text, (float)MM_windowWidth/2, (float)MM_windowHeight/2 - 200.f, 3.0f, GLT_CENTER, GLT_TOP);

    gltSetText(MM_text, "The sound of thunder echoes from below...");
    gltColor(1.0f, 1.0f, 1.0f, textAlpha1);
    gltDrawText2DAligned(MM_text, (float)MM_windowWidth/2, (float)MM_windowHeight/2 - 100.f, 3.0f, GLT_CENTER, GLT_TOP);

    gltSetText(MM_text, "But for now our heroes will rest...");
    gltColor(1.0f, 1.0f, 1.0f, textAlpha2);
    gltDrawText2DAligned(MM_text, (float)MM_windowWidth/2, (float)MM_windowHeight/2 - 0.f, 3.0f, GLT_CENTER, GLT_TOP);

    gltSetText(MM_text, "In these moments of calm, before the storm.");
    gltColor(1.0f, 1.0f, 1.0f, textAlpha2);
    gltDrawText2DAligned(MM_text, (float)MM_windowWidth/2, (float)MM_windowHeight/2 + 100.f, 3.0f, GLT_CENTER, GLT_TOP);

    gltEndDraw();
    if (textAlpha1 < 1.0f && startText1) {
      textAlpha1 += 0.002f;
    } else {
      startText2 = true;
    }
    if (textAlpha2 < 1.0f && startText2) {
      textAlpha2 += 0.002f;
    } else if (startText2) {
      MMSetScreen(2);
    }
}

void CleanUpMainMenu() {
  gltDeleteText(MM_text);
  gltTerminate();
}

