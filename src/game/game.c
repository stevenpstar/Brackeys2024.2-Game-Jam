//standard
#include <math.h>
#include <stdbool.h>
//dependencies
#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#include "../../dep/linmath.h"
//engine
#include "../engine/camera.h"
#include "../engine/shader.h"
#include "../engine/texture.h"
#include "../engine/lighting.h"
#include "../engine/primitives.h"
//game
#include "input.h"
#include "tile.h"
#include "player.h"
#include "string.h"
#include "note.h"
#include "song.h"
#include "game.h"

//flags
bool firstCamMove = false;
// globals

vec3 up = {0.0f, 1.0f, 0.0f};
double mouseX;
double mouseY;
float dt;
float songTime;
GLubyte pixels[32*32*3] = {0};

unsigned int VBO;
unsigned int VAO;
unsigned int shader, lightingShader, uiShader;
unsigned int tilesTexture, wallTexture, levelTexture, blackTexture;
//shader locations
unsigned int viewLoc;
ANote aNotes[1024];
float inputTime;
float nextInputTime;
ANote *nextNote;
int nextNoteIndex = 0;

Note note;

Inputs inputs;
int tiles[1024] = {0};
float planeData[48] = {
  -0.5f, -0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.f, 0.f,//0.0f, // bottom left
   0.5f, -0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 1.f, 0.f, // bottom right
   0.5f,  0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 1.f, 1.f, // top right
   0.5f,  0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
  -0.5f,  0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.f, 1.f, // top left
  -0.5f, -0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.f, 0.f, // bottom left duplicate     
};

Note notePool[30];

int windowWidth, windowHeight;

// Cameras
Camera cam;
Camera fpsCam;
Camera *activeCamera;

// lights
P_CUBE_LIGHT lightCubes[4];

mat4x4 proj, view, model;
vec3 lookAhead;

void InitGame() {
  songTime = 0.0f;
  inputTime = 0.0f;
  windowWidth = 800;
  windowHeight = 600;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  // setup cameras
  vec3 target = {0.0f, 0.0f, 0.0f};
  vec3 pos = {0.0f, 0.0f, -7.0f};
  vec3 fpsPos = {0.0f, 0.4f, 0.0f};
  cam = createCamera(pos, target, 2.5f);
  fpsCam = createCamera(fpsPos, target, 2.5f);
  activeCamera = &fpsCam;

  shader = createShader("./src/shaders/vertex.vert", 
      "./src/shaders/fragment.frag");
  lightingShader = createShader("./src/shaders/vertex.vert", 
      "./src/shaders/lighting.frag");
  uiShader = createShader("./src/shaders/ui.vert",
      "./src/shaders/ui.frag");

  wallTexture = loadTextureRGB("res/wallhue.png");

  setDefaults(&inputs);

  glUseProgram(shader);
  setProjection(shader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(lightingShader);
  setProjection(lightingShader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(shader);

  glUniform1i(glGetUniformLocation(shader, "material.diffuse"), 0);
  glUniform3f(glGetUniformLocation(shader, "material.specular"), 0.5f, 0.5f, 0.5f);
  glUniform1f(glGetUniformLocation(shader, "material.shininess"), 64.0f);

  levelTexture = loadTextureRGB("res/testlevelp.png");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, levelTexture);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
  glGetError();

  blackTexture = loadTextureRGB("res/blacksq.png");

  tilesTexture = createWorld(tiles, "res/floortiles.png", pixels);

  note = (Note){
    .data = {
    -0.1f, -0.1f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
     0.1f, -0.1f, -0.0f,  1.f, 0.f, // bottom right
     0.1f,  0.1f, -0.0f,  1.f, 1.f, // top right
     0.1f,  0.1f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
    -0.1f,  0.1f, -0.0f,  0.f, 1.f, // top left
    -0.1f, -0.1f, -0.0f,  0.f, 0.f, // bottom left duplicate     
    },
    .position = {1.0f, -0.5f},
    .active = true,
    .time = 1.0f,
  };

  // Testing song loading
  readSong("res/songs/song1.txt", aNotes);
  nextInputTime = aNotes[0].time;
  setNextNote(1);
}

void SetupLighting() {
  lightCubes[0] = createCubeLight(VBO, 0.0f, 0.5f, 1.0f);
  lightCubes[1] = createCubeLight(VBO, 2.0f, 0.5f, 1.0f);
  lightCubes[2] = createCubeLight(VBO, 9.0f, 0.5f, 9.0f);
  lightCubes[3] = createCubeLight(VBO, 24.0f, 0.5f, 24.0f);

  vec3 lDir = {-0.2f, -1.0f, -0.3f};
  vec3 lAmb = {0.2f, 0.2f, 0.2f};
  vec3 lDiff = {0.5f, 0.5f, 0.5f};
  setDirectionalLight(lDir, lDiff, lAmb, shader);
  for (int i = 0; i < 4; i++) {
    vec3 lPos = { lightCubes[i].posX, lightCubes[i].posY, lightCubes[i].posZ };
    vec3 diff = { 0.8f, 0.8f, 0.8f };
    setPointLight(i, lPos, diff, shader);
  }
}
void GameUpdate(float deltaTime) {
    dt = deltaTime;
    songTime += deltaTime;
    glBindVertexArray(VAO);
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, activeCamera->position);

    processCameraMovement(activeCamera, &inputs, true, up);

    // camera
    vec3_add(lookAhead, activeCamera->position, activeCamera->direction);
    mat4x4_identity(view);
    mat4x4_translate_in_place(view, activeCamera->position[0], activeCamera->position[1], 
        activeCamera->position[2]);
    mat4x4_look_at(view, activeCamera->position, lookAhead, up);
    viewLoc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view);

    // test rendering world
    glBindBuffer(GL_VERTEX_ARRAY, VBO);
    for (int i=0;i<1024;i++) {
      int row = floor((float)i / 32);
      int col = i - (32 * row);
      setTileData(tiles[i], 16, 112, 112, pixels, planeData, VBO);
      mat4x4 tile;
      mat4x4_identity(tile);
      if (tiles[i] != 34) {
        renderTile(tiles[i], row, col, tile, view, tilesTexture, VBO, shader);
      } else {
        resetTileTexCoords(planeData, VBO);
        renderWall(tiles[i], row, col, tile, view, wallTexture, VBO, shader);
     }
    }

    // testing "strings"
    RenderString(VBO, uiShader, blackTexture, -0.5f);
    RenderString(VBO, uiShader, blackTexture, -0.7f);
    RenderString(VBO, uiShader, blackTexture, -0.9f);
    renderNotes(aNotes, 1024, songTime, VBO,uiShader, blackTexture, nextNoteIndex);
}

void DeleteBuffers() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void mouseMove(GLFWwindow* window, double xpos, double ypos) {
  if (firstCamMove) {
    mouseX = xpos;
    mouseY = ypos;
    firstCamMove = false;
  }
  // This is for camera mouse look (fps, spectator camera etc.)
  // Will need to be gated behind some flag at some point
  float xoffset = xpos - mouseX;
  float yoffset = mouseY - ypos;  
  mouseX = xpos;
  mouseY = ypos;

  mouseLook(xoffset, yoffset, activeCamera, dt);
}

void setNextNote(int string) {
  // TODO: Magic numbers
  for (int i = 0; i < 1024; i++) {
    if (aNotes[i].string == -1) {
      return;
    }
    if (aNotes[i].string == string && aNotes[i].time > songTime) {
      nextNote = &aNotes[i];
      nextNoteIndex = i;
      aNotes[i].colour[0] = 0.0f;
      aNotes[i].colour[1] = 0.0f;
      aNotes[i].colour[2] = 1.0f;
      printf("Next note has been set: %d\n", nextNoteIndex);
      return;
    }
  }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
    songTime = 0.0f;
    inputTime = 0.0f;
  }
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    inputTime = songTime;
    // 0.225f is a buffer for where the indicator is (to time your presses)
    float nextNoteTime = songTime - nextNote->time;
    float difference = fabsf(nextNoteTime - 1.0f);
    if (difference < 0.2) {
      aNotes[nextNoteIndex].colour[0] = 0.0f;
      aNotes[nextNoteIndex].colour[1] = 1.0f;
      aNotes[nextNoteIndex].colour[2] = 0.0f;
    } else {
      aNotes[nextNoteIndex].colour[0] = 1.0f;
      aNotes[nextNoteIndex].colour[1] = 0.0f;
      aNotes[nextNoteIndex].colour[2] = 0.0f;
    }
    setNextNote(1);
//    printf("difference: %f\n", difference);
//    printf("inputTime: %f, nextInputTime: %f\n", inputTime, nextInputTime);
//    printf("test: %f\n", nextNoteTime);
  }
  if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    inputs.forward.Down = true;
  } else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
    inputs.forward.Down = false;
  }
  if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    inputs.backwards.Down = true;
  } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
    inputs.backwards.Down = false;
  }
  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    inputs.left.Down = true;
  } else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
    inputs.left.Down = false;
  }
  if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    inputs.right.Down = true;
  } else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
    inputs.right.Down = false;
  }
}

void resizeWindow(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  windowWidth = width;
  windowHeight = height;
  glUseProgram(shader);
  setProjection(shader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(lightingShader);
  setProjection(lightingShader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(shader);
}



