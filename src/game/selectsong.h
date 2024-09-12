#ifndef H_SELECTSONG
#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#define H_SELECTSONG
void SM_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void SM_mouseMove(GLFWwindow* window, double xpos, double ypos);
void SM_resizeWindow(GLFWwindow* window, int width, int height);

void InitSelectSong(void (*SetScreen)(int), void (*SetSelectedSong)(char[512]), int w, int h);
void UpdateSelectSong(float deltaTime);
void CleanUpSelectSong();
void SetSongList(char songs[8][512]);
int GetSongCount();

#endif
