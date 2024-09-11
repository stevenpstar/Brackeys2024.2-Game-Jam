//standard
#include <math.h>
#include <stdbool.h>
//dependencies
#define MINIAUDIO_IMPLEMENTATION
#include "../../dep/miniaudio.h"
#include "../../dep/glad/glad.h"
#include "../../dep/GLFW/glfw3.h"
#include "../../dep/linmath.h"
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "../../dep/gltext.h"
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
#include "key.h"

//flags
bool firstCamMove = false;
bool stringOneDown = false;
bool stringTwoDown = false;
bool stringThreeDown = false;
bool stringFourDown = false;
bool stringFiveDown = false;
bool stringSixDown = false;

// globals
ma_result result;
ma_engine engine;

vec3 up = {0.0f, 1.0f, 0.0f};
double mouseX;
double mouseY;
float dt;
float songTime;
GLubyte pixels[32*32*3] = {0};

// Characters
Sprite bardSprite;
Player bard;
Animation bardAnim = {
  .state = IDLE_DOWN,
  .startFrame = 0,
  .endFrame = 1,
};

Sprite rogueSprite;
Player rogue;
Animation rogueAnim = {
  .state = IDLE_DOWN,
  .startFrame = 0,
  .endFrame = 1,
};

Sprite wizardSprite;
Player wizard;
Animation wizardAnim = {
  .state = IDLE_DOWN,
  .startFrame = 0,
  .endFrame = 1,
};

Sprite knightSprite;
Player knight;
Animation knightAnim = {
  .state = IDLE_DOWN,
  .startFrame = 0,
  .endFrame = 1,
};

unsigned int VBO;
unsigned int VAO;
unsigned int shader, lightingShader, uiShader;
unsigned int tilesTexture, wallTexture, wallTextureTop, levelTexture, blackTexture, whiteTexture;
UISprite aKey, sKey, dKey, jKey, kKey, lKey;
//shader locations
unsigned int viewLoc;
ANote aNotes[1024];
float inputTimes[6] = {0};
float nextInputTime;
ANote *nextNote;
int nextNoteIndex[6] = {0};

Note note;
int totalScore = 0;
int currentScore = 0;

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

//Text
GLTtext *text;

void InitGame() {
  gltInit();
  text = gltCreateText();
  result = ma_engine_init(NULL, &engine);
  if (result != MA_SUCCESS) {
    printf("MiniAudio failed to init\n");
    return;
  }
  songTime = 0.0f;
  inputTimes[0] = 0.0f;
  inputTimes[1] = 0.0f;
  inputTimes[2] = 0.0f;
  nextNoteIndex[0] = -1;
  nextNoteIndex[1] = -1;
  nextNoteIndex[2] = -1;
  windowWidth = 800;
  windowHeight = 600;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  // setup cameras
  vec3 target = {0.0f, 0.0f, 0.0f};
  vec3 pos = {0.0f, 0.0f, -7.0f};
  vec3 fpsPos = {11.f, 2.5f, 8.0f};
  cam = createCamera(pos, target, 2.5f, -90.0f, 0.0f);
  fpsCam = createCamera(fpsPos, target, 2.5f, 180.0f, -35.0f);
  activeCamera = &fpsCam;

  shader = createShader("./src/shaders/vertex.vert", 
      "./src/shaders/fragment.frag");
  lightingShader = createShader("./src/shaders/vertex.vert", 
      "./src/shaders/lighting.frag");
  uiShader = createShader("./src/shaders/ui.vert",
      "./src/shaders/ui.frag");

  wallTexture = loadTextureRGB("res/forestwall16bottom.png");
  wallTextureTop = loadTextureRGB("res/treetop.png");
  vec3 aKeyPos = { -0.8f, -0.4f, -0.1f };
  vec3 sKeyPos = { -0.8f, -0.5f, -0.1f };
  vec3 dKeyPos = { -0.8f, -0.6f, -0.1f };
  vec3 jKeyPos = { -0.8f, -0.7f, -0.1f };
  vec3 kKeyPos = { -0.8f, -0.8f, -0.1f };
  vec3 lKeyPos = { -0.8f, -0.9f, -0.1f };
  aKey = createAnimatedUI(VBO, aKeyPos,"res/akey.png", 32, 32, 64, 32);
  sKey = createAnimatedUI(VBO, sKeyPos,"res/skey.png", 32, 32, 64, 32);
  dKey = createAnimatedUI(VBO, dKeyPos,"res/dkey.png", 32, 32, 64, 32);
  jKey = createAnimatedUI(VBO, jKeyPos,"res/jkey.png", 32, 32, 64, 32);
  kKey = createAnimatedUI(VBO, kKeyPos,"res/kkey.png", 32, 32, 64, 32);
  lKey = createAnimatedUI(VBO, lKeyPos,"res/lkey.png", 32, 32, 64, 32);

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
  whiteTexture = loadTextureRGB("res/whitesq.png");

  tilesTexture = createWorld(tiles, "res/foresttiles.png", pixels);

  // Character setup
  bardSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/bardchar.png", 
      16, 16, 32, 32);
  bard.x = 8.f;
  bard.y = 1.f;
  bard.z = 8.f;
  bard.sprite = &bardSprite;
  bard.anim = &bardAnim;
  bard.state = 0;
  bard.framerate = 12;
  bard.frameTimer = 0.0f;

  rogueSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/roguechar.png", 
      16, 16, 32, 32);
  rogue.x = 8.f;
  rogue.y = 1.f;
  rogue.z = 9.f;
  rogue.sprite = &rogueSprite;
  rogue.anim = &rogueAnim;
  rogue.state = 0;
  rogue.framerate = 12;
  rogue.frameTimer = 0.0f;

  wizardSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/prototype_character.png", 
      32, 32, 128, 384);
  wizard.x = 8.f;
  wizard.y = 1.f;
  wizard.z = 7.f;
  wizard.sprite = &wizardSprite;
  wizard.anim = &wizardAnim;
  wizard.state = 0;
  wizard.framerate = 8;
  wizard.frameTimer = 0.0f;

  knightSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/prototype_character.png", 
      32, 32, 128, 384);
  knight.x = 9.f;
  knight.y = 1.f;
  knight.z = 8.f;
  knight.sprite = &knightSprite;
  knight.anim = &knightAnim;
  knight.state = 0;
  knight.framerate = 8;
  knight.frameTimer = 0.0f;

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
  readSong("res/songs/song1.txt", aNotes, &totalScore);
  resetSong();
}

void SetupLighting() {
  lightCubes[0] = createCubeLight(VBO, 0.0f, 0.5f, 1.0f);
  lightCubes[1] = createCubeLight(VBO, 2.0f, 0.5f, 1.0f);
  lightCubes[2] = createCubeLight(VBO, 10.0f, 0.5f, 11.0f);
  lightCubes[3] = createCubeLight(VBO, 24.0f, 0.5f, 24.0f);

  vec3 lDir = {-0.2f, -1.0f, -0.3f};
  vec3 lAmb = {0.1f, 0.1f, 0.1f};
  vec3 lDiff = {0.2f, 0.2f, 0.2f};
  setDirectionalLight(lDir, lDiff, lAmb, shader);
  for (int i = 0; i < 4; i++) {
    vec3 lPos = { lightCubes[i].posX, lightCubes[i].posY, lightCubes[i].posZ };
    vec3 diff;
    if (i == 2) {
      vec3 diff = { 1.f, 0.0f, 0.0f };
      setPointLight(i, lPos, diff, shader);
    }
    else {
      vec3 diff = { 0.2f, 0.2f, 0.2f };
      setPointLight(i, lPos, diff, shader);
    }
  }
}
void GameUpdate(float deltaTime) {
    dt = deltaTime;
    songTime += deltaTime;
    glBindVertexArray(VAO);
    glBindBuffer(GL_VERTEX_ARRAY, VBO);
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, activeCamera->position);

    //processCameraMovement(activeCamera, &inputs, true, up);

    // camera
    vec3_add(lookAhead, activeCamera->position, activeCamera->direction);
    mat4x4_identity(view);
    mat4x4_translate_in_place(view, activeCamera->position[0], activeCamera->position[1], 
        activeCamera->position[2]);
    mat4x4_look_at(view, activeCamera->position, lookAhead, up);
    viewLoc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view);


    // test rendering world
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
        renderWall(tiles[i], row, col, tile, view, wallTexture, blackTexture, VBO, shader);
     }
    }

    // render characters
    glUseProgram(shader);
    mat4x4 model;
    mat4x4_identity(model);
    mat4x4_translate_in_place(model, bard.x, bard.y, bard.z);
    mat4x4_scale_aniso(model, model, 0.5f, 0.5f, 0.5f);
    mat4x4_rotate_Y(model, model, degToRad(90.0f));
    unsigned int modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bardSprite.texture);
    Animate(&bard, bardAnim, true, deltaTime, VBO);
   // SetFrame(&bardSprite, 0, VBO, false);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    mat4x4_identity(model);
    mat4x4_translate_in_place(model, rogue.x, rogue.y, rogue.z);
    mat4x4_scale_aniso(model, model, 0.5f, 0.5f, 0.5f);
    mat4x4_rotate_Y(model, model, degToRad(90.0f));
    modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rogueSprite.texture);
    Animate(&rogue, rogueAnim, true, deltaTime, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    mat4x4_identity(model);
    mat4x4_translate_in_place(model, wizard.x, wizard.y, wizard.z);
    mat4x4_rotate_Y(model, model, degToRad(90.0f));
    modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wizardSprite.texture);
    Animate(&wizard, wizardAnim, true, deltaTime, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    mat4x4_identity(model);
    mat4x4_translate_in_place(model, knight.x, knight.y, knight.z);
    mat4x4_rotate_Y(model, model, degToRad(90.0f));
    modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, knightSprite.texture);
    Animate(&knight, knightAnim, true, deltaTime, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(uiShader);

    RenderString(VBO, uiShader, whiteTexture, -0.45f, windowWidth);
    RenderString(VBO, uiShader, whiteTexture, -0.55f, windowWidth);
    RenderString(VBO, uiShader, whiteTexture, -0.65f, windowWidth);
    RenderString(VBO, uiShader, whiteTexture, -0.75f, windowWidth);
    RenderString(VBO, uiShader, whiteTexture, -0.85f, windowWidth);
    RenderString(VBO, uiShader, whiteTexture, -0.95f, windowWidth);

    renderNotes(aNotes,
        1024,
        songTime,
        VBO,
        uiShader,
        whiteTexture,
        nextNoteIndex,
        setNextNote,
        windowWidth,
        windowHeight);

    renderKey(&aKey,
        stringOneDown,
        VBO,
        aKey.texture,
        uiShader,
        windowWidth,
        windowHeight);
    renderKey(&sKey,
        stringTwoDown,
        VBO,
        sKey.texture,
        uiShader,
        windowWidth,
        windowHeight);
    renderKey(&dKey,
        stringThreeDown,
        VBO,
        dKey.texture,
        uiShader,
        windowWidth,
        windowHeight);
    renderKey(&jKey,
        stringFourDown,
        VBO,
        jKey.texture,
        uiShader,
        windowWidth,
        windowHeight);
    renderKey(&kKey,
        stringFiveDown,
        VBO,
        kKey.texture,
        uiShader,
        windowWidth,
        windowHeight);
    renderKey(&lKey,
        stringSixDown,
        VBO,
        lKey.texture,
        uiShader,
        windowWidth,
        windowHeight);
    gltBeginDraw();
    char str[128];

  //  gltSetText(text, "Score: ");
  //  gltColor(1.0f, 1.0f, 1.0f, 1.0f);
  //  gltDrawText2D(text, 0.5f, 0.5f, 4.0f);

    gltSetText(text, itoa(currentScore, str, 10));
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(text, 10.f, 0.5f, 4.0f);

    gltSetText(text, "/");
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(text, 80.f, 0.5f, 4.0f);

    gltSetText(text, itoa(totalScore, str, 10));
    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText2D(text, 120.f, 0.5f, 4.0f);

    gltEndDraw();
}

void DeleteBuffers() {
  ma_engine_uninit(&engine);
  gltDeleteText(text);
  gltTerminate();
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

  //mouseLook(xoffset, yoffset, activeCamera, dt);
}

void setNextNote(int string) {
  // TODO: Magic numbers
  for (int i = 0; i < 1024; i++) {
    if (aNotes[i].string == -1) {
      return;
    }
    if (aNotes[i].string == string && aNotes[i].active) {
      nextNote = &aNotes[i];
      nextNoteIndex[string-1] = i;
      aNotes[i].colour[0] = 0.0f;
      aNotes[i].colour[1] = 0.0f;
      aNotes[i].colour[2] = 1.0f;
      return;
    } else if (aNotes[i].string == string && aNotes[i].active && i > nextNoteIndex[string-1]) {
      // set to neutral colour
      aNotes[i].colour[0] = 0.0f;
      aNotes[i].colour[1] = 0.0f;
      aNotes[i].colour[2] = 0.0f;
    }
  }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
    resetSong();
  }
  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    if (mods == 1) {
      playString(1, "res/a4.wav", true);
    } else {
      playString(1, "res/a3.wav", false);
    }
    stringOneDown = true;
  } 
  else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
    stringOneDown = false;
  }
  if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    if (mods == 1) {
      playString(2, "res/c5.wav", true);
    } else {
      playString(2, "res/c4.wav", false);
    }
    stringTwoDown = true;
  } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
    stringTwoDown = false;
  }
  if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    if (mods == 1) {
      playString(3, "res/d5.wav", true);
    } else {
      playString(3, "res/d4.wav", false);
    }
    stringThreeDown = true;
  }
  else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
    stringThreeDown = false;
  }
  if (key == GLFW_KEY_J && action == GLFW_PRESS) {
    if (mods == 1) {
      playString(4, "res/e5.wav", true);
    } else {
      playString(4, "res/e4.wav", false);
    }
    stringFourDown = true;
  }
  else if (key == GLFW_KEY_J && action == GLFW_RELEASE) {
    stringFourDown = false;
  }
  if (key == GLFW_KEY_K && action == GLFW_PRESS) {
    if (mods == 1) {
      playString(5, "res/f5.wav", true);
    } else {
      playString(5, "res/f4.wav", false);
    }
    stringFiveDown = true;
  }
  else if (key == GLFW_KEY_K && action == GLFW_RELEASE) {
    stringFiveDown = false;
  }
  if (key == GLFW_KEY_L && action == GLFW_PRESS) {
    if (mods == 1) {
      playString(6, "res/g5.wav", true);
    } else {
      playString(6, "res/g4.wav", false);
    }
    stringSixDown = true;
  }
  else if (key == GLFW_KEY_L && action == GLFW_RELEASE) {
    stringSixDown = false;
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
  gltViewport(width, height);
  windowWidth = width;
  windowHeight = height;
  glUseProgram(shader);
  setProjection(shader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(lightingShader);
  setProjection(lightingShader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(shader);
}

void resetSong() {
  songTime = 0.0f;
  currentScore = 0;
  // reset all notes
  for (int i = 0; i < 1024; i++) {
    aNotes[i].active = true;
    aNotes[i].colour[0] = 0.0f;
    aNotes[i].colour[1] = 0.0f;
    aNotes[i].colour[2] = 0.0f;
  }
  inputTimes[0] = 0.0f;
  inputTimes[1] = 0.0f;
  inputTimes[2] = 0.0f;
  inputTimes[3] = 0.0f;
  inputTimes[4] = 0.0f;
  inputTimes[5] = 0.0f;

  setNextNote(1);
  setNextNote(2);
  setNextNote(3);
  setNextNote(4);
  setNextNote(5);
  setNextNote(6);
}

void playString(int string, const char *noteFile, bool octave) {

  ma_engine_play_sound(&engine, noteFile, NULL);
  inputTimes[string-1] = songTime - aNotes[nextNoteIndex[string-1]].time;
  float difference = fabsf(inputTimes[string-1] - 1.0f);
  if (difference < 0.2) {
    currentScore += 2;
    aNotes[nextNoteIndex[string-1]].colour[0] = 0.0f;
    aNotes[nextNoteIndex[string-1]].colour[1] = 1.0f;
    aNotes[nextNoteIndex[string-1]].colour[2] = 0.0f;
    aNotes[nextNoteIndex[string-1]].active = false;
  } else {
    aNotes[nextNoteIndex[string-1]].colour[0] = 1.0f;
    aNotes[nextNoteIndex[string-1]].colour[1] = 0.0f;
    aNotes[nextNoteIndex[string-1]].colour[2] = 0.0f;
    aNotes[nextNoteIndex[string-1]].active = false;
  }
  setNextNote(string);
}


