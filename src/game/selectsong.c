//dep
#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "../../dep/gltext.h"
#include "../../dep/linmath.h"
//std
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//game
#include "selectsong.h"
#include "key.h"
//engine
#include "../engine/primitives.h"
#include "../engine/shader.h"

int pages;
int currentPage;
int songCount;
int selectedSong;
// window width/height
int SM_width;
int SM_height;
char songList[200][512];

GLTtext *SM_text;
void (*SMSetScreen)(int);
void (*SMSetSong)(char[512]);

UISprite SM_escKey;
unsigned int SM_VBO, SM_VAO;
unsigned int SM_uiShader;

void SM_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    SMSetScreen(2);
  }
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    if (selectedSong < songCount) {
      selectedSong++;
    }
  }
  if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    if (selectedSong > 0) {
      selectedSong--;
    }
  }
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS || key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    SMSetSong(songList[selectedSong]);
    SMSetScreen(2);
  }

}
void SM_mouseMove(GLFWwindow* window, double xpos, double ypos) {}
void SM_resizeWindow(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  gltViewport(width, height);
  SM_width = width;
  SM_height = height;
}

void InitSelectSong(void (*SetScreen)(int), void (*SetSelectedSong)(char[512]), int w, int h) {
  gltInit();

  SM_text = gltCreateText();
  glGenVertexArrays(1, &SM_VAO);
  glGenBuffers(1, &SM_VBO);
  glBindVertexArray(SM_VAO);

  songCount = GetSongCount();
  selectedSong = 0;
  pages = floor((float)songCount / 8);
  printf("Pages: %d\n", pages);
  currentPage = 0;
  SMSetScreen = SetScreen;
  SMSetSong = SetSelectedSong;
  SetSongList(songList);
  glViewport(0, 0, w, h);
  gltViewport(w, h);
  SM_width = w;
  SM_height = h;

  SM_uiShader = createShader("./src/shaders/ui.vert",
      "./src/shaders/ui.frag");

  vec3 escKeyPos = { -0.95, 0.95f, -0.1f};
  SM_escKey = createAnimatedUI(SM_VBO, escKeyPos,"res/esckey.png", 32, 32, 64, 32);

}
void UpdateSelectSong(float deltaTime) {
  glUseProgram(SM_uiShader);
//  renderKey(&SM_escKey,
//      false,
//      SM_VBO,
//      SM_escKey.texture,
//      SM_uiShader,
//      SM_width,
//      SM_height, 0.1f);

  gltBeginDraw();
  for (int i = 0;i<200;i++) {
    gltSetText(SM_text, songList[i]);
    if (selectedSong == i) {
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
      gltColor(0.6f, 0.6f, 0.6f, 1.0f);
    }
    gltDrawText2DAligned(SM_text,
        (float)SM_width / 2, ((float)SM_height / 2) - (selectedSong - i) * 80.f,
        3.0f, GLT_CENTER, GLT_TOP);
  }
  gltEndDraw();

}

void CleanUpSelectSong() {
  gltDeleteText(SM_text);
  gltTerminate();
  glDeleteVertexArrays(1, &SM_VAO);
  glDeleteBuffers(1, &SM_VBO);

}
void SetSongList(char songs[200][512]) {
  for (int i=0;i<200;i++) {
    // clear songs array before populating
    strcpy(songs[i], "");
  }
  FILE *fp;
  fp = fopen("res/songlist.txt", "r");
  char s[1024];
  int songCount = 0;
  int songIndex = 0;
  if (fp == NULL) {
    printf("Song List file not found\n");
  } else {
    while (fgets(s, sizeof s, fp) != NULL) {
      strcpy(songs[songIndex], s);
      songs[songIndex][strcspn(songs[songIndex], "\n")] = 0;
      songIndex++;
      songCount++;
      printf("S: %s\n", s);
    }
  }
  fclose(fp);

}
int GetSongCount() {
  FILE *fp;
  fp = fopen("res/songlist.txt", "r");
  char s[1024];
  int songCount = 0;
  if (fp == NULL) {
    printf("Song List file not found\n");
    return 0;
  }
  while (fgets(s, sizeof s, fp) != NULL) {
    songCount++;
  }
  fclose(fp);
  return songCount;
}

