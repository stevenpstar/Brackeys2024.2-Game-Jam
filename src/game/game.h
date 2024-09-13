#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#include "../../dep/miniaudio.h"

#ifndef H_GAME
#define H_GAME

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouseMove(GLFWwindow* window, double xpos, double ypos);
void resizeWindow(GLFWwindow* window, int width, int height);

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void InitGame(void (*SetScreen)(int), char selectedSong[512], int w, int h);
void SetupLighting();
void GameUpdate(float deltaTime);
void DeleteBuffers();
void setNextNote(int string);
void playString(int string, const char *noteFile, bool octave);
void resetSong(void);
void moveCamera(void);
void setSongEnded();
#endif
