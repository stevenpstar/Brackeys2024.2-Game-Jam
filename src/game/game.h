#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"

#ifndef H_GAME
#define H_GAME

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouseMove(GLFWwindow* window, double xpos, double ypos);
void resizeWindow(GLFWwindow* window, int width, int height);

void InitGame();
void SetupLighting();
void GameUpdate(float deltaTime);
void DeleteBuffers();
void setNextNote(int string);
void playString(int string, const char *noteFile);
void resetSong();
#endif
