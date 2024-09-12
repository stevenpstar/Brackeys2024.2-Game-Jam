//dep
#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "../../dep/gltext.h"
//std
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//game
#include "selectsong.h"

int pages;
int currentPage;
int songCount;
int selectedSong;
// window width/height
int SM_width;
int SM_height;
char songList[8][512];

GLTtext *SM_text;
void (*SMSetScreen)(int);
void (*SMSetSong)(char[512]);

void SM_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    // TODO: This should probably go back to main menu
    SMSetScreen(0);
  }
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    if (currentPage > 0) {
      currentPage--;
      selectedSong = 0;
      SetSongList(songList);
    }
  }
  else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    if (currentPage < pages) {
      currentPage++;
      selectedSong = 0;
      SetSongList(songList);
    }
  }
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    if (selectedSong < 7) {
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

void InitSelectSong(void (*SetScreen)(int), void (*SetSelectedSong)(char[512])) {
  songCount = GetSongCount();
  selectedSong = 0;
  pages = floor((float)songCount / 8);
  printf("Pages: %d\n", pages);
  currentPage = 0;
  gltInit();
  SM_text = gltCreateText();
  SMSetScreen = SetScreen;
  SMSetSong = SetSelectedSong;
  SetSongList(songList);
}
void UpdateSelectSong(float deltaTime) {
  gltBeginDraw();
  for (int i = 0;i<8;i++) {
    gltSetText(SM_text, songList[i]);
    if (selectedSong == i) {
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    } else {
      gltColor(0.6f, 0.6f, 0.6f, 1.0f);
    }
    gltDrawText2D(SM_text, (float)SM_width / 2, 120.0f + (80.0f * i), 3.0f);
  }
  gltSetText(SM_text, "-");
  gltColor(1.0f, 1.0f, 1.0f, 1.0f);
  gltDrawText2D(SM_text, (float)SM_width / 2 - 50.f, 120.0f + (80.0f * selectedSong), 3.0f);
  gltEndDraw();
}

void CleanUpSelectSong() {
  gltDeleteText(SM_text);
  gltTerminate();
}
void SetSongList(char songs[8][512]) {
  for (int i=0;i<8;i++) {
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
      if (songCount >= currentPage * 8 && songCount < (currentPage * 8) + 8) {
        strcpy(songs[songIndex], s);
        songs[songIndex][strcspn(songs[songIndex], "\n")] = 0;
        songIndex++;
      }
      songCount++;
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

