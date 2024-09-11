#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#include "../../dep/linmath.h"
#include "songeditor.h"
#include "../engine/shader.h"
#include "../engine/texture.h"
#include <stdio.h>
#include <stdbool.h>

// globals
int zoomLevel = 1;
vec3 sqColour = { 1.0f, 1.0f, 1.0f };
vec3 pickColour = { 1.0f, 0.0f, 0.0f };
vec2 sqPosition = { 0.0f, 0.0f };

unsigned int SE_uiShader;
unsigned int SE_VBO;
unsigned int SE_VAO;

unsigned int SE_whiteTexture;

int SE_windowWidth;
int SE_windowHeight;

float sqWidth = 2.0 / 32.f;
SESquare oneStringSq[32];

float plane[30] = {
  -0.5f, -0.5f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
   0.5f, -0.5f, -0.0f,  1.f, 0.f, // bottom right
   0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right
   0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
  -0.5f,  0.5f, -0.0f,  0.f, 1.f, // top left
  -0.5f, -0.5f, -0.0f,  0.f, 0.f, // bottom left duplicate     
};

float data[30] = {
  -0.0f, -0.0f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
   0.5f, -0.0f, -0.0f,  1.f, 0.f, // bottom right
   0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right
   0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
  -0.0f,  0.5f, -0.0f,  0.f, 1.f, // top left
  -0.0f, -0.0f, -0.0f,  0.f, 0.f, // bottom left duplicate     
};

void InitSongEditor() {
  SE_windowWidth = 800;
  SE_windowHeight = 600;
  glGenVertexArrays(1, &SE_VAO);
  glGenBuffers(1, &SE_VBO);
  glBindVertexArray(SE_VAO);

  SE_uiShader = createShader("./src/shaders/ui.vert",
      "./src/shaders/ui.frag");

  SE_whiteTexture = loadTextureRGB("res/whitesq.png");
  glBindBuffer(GL_ARRAY_BUFFER, SE_VBO);
  glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  for (int i = 0;i<32;i++) {
    oneStringSq[i].position[0] = -1.0f + (i * sqWidth);
    oneStringSq[i].position[1] = 0.0f;
    oneStringSq[i].dim = sqWidth;
    if (i % 2 == 0)  {
      oneStringSq[i].colour[0] = 1.0f;
      oneStringSq[i].colour[1] = 1.0f;
      oneStringSq[i].colour[2] = 1.0f;
    } else {
      oneStringSq[i].colour[0] = 0.8f;
      oneStringSq[i].colour[1] = 0.8f;
      oneStringSq[i].colour[2] = 0.8f;
    }
  }

  printf("Init song editor\n");
}
void UpdateSongEditor(float deltaTime) {
  RenderGrid();
}
void RenderGrid() {
  glBindVertexArray(SE_VAO);
  glUseProgram(SE_uiShader);
  // doing test of mouse clicking ui element
  for (int i = 0; i < 32; i++) {
    mat4x4 model;
    mat4x4_identity(model);
    mat4x4_translate_in_place(model, oneStringSq[i].position[0], oneStringSq[i].position[1], -0.1f);
    mat4x4_scale_aniso(model, model, sqWidth * 2, sqWidth, 0.0f);
    unsigned int modelLoc = glGetUniformLocation(SE_uiShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(SE_uiShader, "col"),
        oneStringSq[i].colour[0],
        oneStringSq[i].colour[1],
        oneStringSq[i].colour[2]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SE_whiteTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}

void CleanUpSongEditor() {
  glDeleteVertexArrays(1, &SE_VAO);
  glDeleteBuffers(1, &SE_VBO);
}

void SE_resizeWindow(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
//  gltViewport(width, height);
  SE_windowWidth = width;
  SE_windowHeight = height;
 }

void SE_mouseMove(GLFWwindow* window, double xpos, double ypos) {
  // getting x position of square
  //    int xStart = SE_windowWidth * ((sqPosition[0] + 1.0f) / 2.0f);
   // int width = SE_windowWidth * (0.5f / 2.0f);
   // int yStart = SE_windowHeight * ((sqPosition[1] + 1.0f) / 2.0f);
   // int height = SE_windowHeight * (0.5f / 2.0f);

  for (int i = 0; i < 32; i++) {
    int xStart = SE_windowWidth * ((oneStringSq[i].position[0] + 1.0f) / 2.0f);
    int width = SE_windowWidth * (sqWidth / 2.0f);
    int yStart = SE_windowHeight * ((oneStringSq[i].position[1] + 1.0f) / 2.0f);
    int height = SE_windowHeight * (sqWidth / 2.0f);
   // printf("xpos + width: %d\n", xStart + width);
    if (xpos >= xStart && xpos <= xStart + width
        && ypos < yStart && ypos >= yStart - height) {
      oneStringSq[i].colour[0] = 1.0f;
      oneStringSq[i].colour[1] = 0.0f;
      oneStringSq[i].colour[2] = 0.0f;
    }
    else  {
      if (i % 2 == 0) {
        oneStringSq[i].colour[0] = 1.0f;
        oneStringSq[i].colour[1] = 1.0f;
        oneStringSq[i].colour[2] = 1.0f;
      } else {
        oneStringSq[i].colour[0] = 0.8f;
        oneStringSq[i].colour[1] = 0.8f;
        oneStringSq[i].colour[2] = 0.8f;
      }
    }
  }
}

void SE_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}


