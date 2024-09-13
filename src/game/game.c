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

#define CAMERA_COUNT 3

//flags
bool firstCamMove = false;
bool stringOneDown = false;
bool stringTwoDown = false;
bool stringThreeDown = false;
bool stringFourDown = false;
bool stringFiveDown = false;
bool stringSixDown = false;

bool guitarRotateUp = true;

bool songStarted = false;
bool songEnded = false;
bool menuOpen = true;
bool showScore = false;
bool fullScreen = true;

void (*GameSetScreen)(int);

// menu vars
int selMenuIndex = 0;

// globals
ma_result result;
ma_result fireResult;
ma_engine engine;
ma_decoder decoder;
ma_device device;
ma_device_config deviceConfig;

vec3 up = {0.0f, 1.0f, 0.0f};
double mouseX;
double mouseY;
float dt;
float songTime;
GLubyte pixels[32*32*3] = {0};
int cameraIndex = 0;
int cameraMoveTicker = 0;
int cameraMoveTime = 1200;
vec3 cameraPositions[CAMERA_COUNT] = {
  {10.f, 1.f, 9.0f},
  {8.f, 0.2f, 8.0f},
  {12.f, 5.f, 3.0f},
};
// yaw, pitch, roll (for whatever reason)
vec3 cameraRotations[CAMERA_COUNT] = {
  {197.f, -30.0f, 0.0f},
  {180.f, 0.0f, 0.0f},
  {135.f, -75.0f, 0.0f},
};

// Characters
Sprite bardSprite;
Player bard;
Animation bardAnim = {
  .state = IDLE_DOWN,
  .startFrame = 0,
  .endFrame = 1,
};

// this doesn't really need to be a player but seems easier 
Sprite guitarSprite;
Player guitar;
Animation guitarAnim = {
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
unsigned int tilesTexture, wallTexture, wallFlagTexture, levelTexture, blackTexture, whiteTexture;
// note textures
unsigned int blueNoteTex, blueNoteOctTex;
unsigned int greenNoteTex, greenNoteOctTex;
unsigned int redNoteTex, redNoteOctTex;
unsigned int noteEffectTex;
UISprite aKey, sKey, dKey, jKey, kKey, lKey, noteEffect;
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
int lastScore = 0;

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

void InitGame(void (*SetScreen)(int), char selectedSong[512], int w, int h) {
  GameSetScreen = SetScreen;
  gltInit();
  text = gltCreateText();
  result = ma_engine_init(NULL, &engine);

  fireResult = ma_decoder_init_file("res/thunderstorm.mp3", NULL, &decoder);
    if (result != MA_SUCCESS) {
      printf("fire result issue!\n");
        return;
    }
    
  ma_data_source_set_looping(&decoder, MA_TRUE);

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format   = decoder.outputFormat;
  deviceConfig.playback.channels = decoder.outputChannels;
  deviceConfig.sampleRate        = decoder.outputSampleRate;
  deviceConfig.dataCallback      = data_callback;
  deviceConfig.pUserData         = &decoder;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
      printf("Failed to open playback device.\n");
      ma_decoder_uninit(&decoder);
      printf("MiniAudio failed to init decoder\n");
      return;
  }
  if (ma_device_set_master_volume(&device, 0.1f) != MA_SUCCESS) {
    printf("Volume not set!\n");
  }

  if (ma_device_start(&device) != MA_SUCCESS) {
      printf("Failed to start playback device.\n");
      ma_device_uninit(&device);
      ma_decoder_uninit(&decoder);
      printf("MiniAudio failed to init device start\n");
      return;
  }
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
  vec3 fpsPos = {10.f, 1.f, 9.0f};
  cam = createCamera(pos, target, 2.5f, -30.0f, 0.0f);
  fpsCam = createCamera(fpsPos, target, 2.5f, 197.f, -30.0f);
  activeCamera = &fpsCam;

  shader = createShader("./src/shaders/vertex.vert", 
      "./src/shaders/fragment.frag");
  lightingShader = createShader("./src/shaders/vertex.vert", 
      "./src/shaders/lighting.frag");
  uiShader = createShader("./src/shaders/ui.vert",
      "./src/shaders/ui.frag");

  wallTexture = loadTextureRGB("res/wallhue.png");
  wallFlagTexture = loadTextureRGB("res/wallhueflag.png");
  vec3 aKeyPos = { -0.8f, -0.4f, -0.1f };
  vec3 sKeyPos = { -0.8f, -0.5f, -0.1f };
  vec3 dKeyPos = { -0.8f, -0.6f, -0.1f };
  vec3 jKeyPos = { -0.8f, -0.7f, -0.1f };
  vec3 kKeyPos = { -0.8f, -0.8f, -0.1f };
  vec3 lKeyPos = { -0.8f, -0.9f, -0.1f };
  noteEffect = createAnimatedUI(VBO, aKeyPos,"res/noteEffect.png", 32, 32, 128, 32);
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
  tilesTexture = createWorld(tiles, "res/floortiles.png", pixels);

  blueNoteTex = loadTexture("res/notebnorm.png");
  blueNoteOctTex = loadTexture("res/noteboctave.png");
  greenNoteTex = loadTexture("res/notegnorm.png");
  greenNoteOctTex = loadTexture("res/notegoctave.png");
  redNoteTex = loadTexture("res/noternorm.png");
  redNoteOctTex = loadTexture("res/noteroctave.png");

  noteEffectTex = loadTexture("res/noteeffect.png");

  // Character setup
  bardSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/bardchar.png", 
      16, 16, 32, 32);
  bard.x = 6.f;
  bard.y = 0.25f;
  bard.z = 8.f;
  bard.sprite = &bardSprite;
  bard.anim = &bardAnim;
  bard.state = 0;
  bard.framerate = 12;
  bard.frameTimer = 0.0f;

  guitarSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/guitar16anim.png", 
      16, 16, 32, 16);
  guitar.x = 8.f;
  guitar.y = 0.25f;
  guitar.z = 8.f;
  guitar.sprite = &guitarSprite;
  guitar.anim = &guitarAnim;
  guitar.state = 0;
  guitar.framerate = 12;
  guitar.frameTimer = 0.0f;

  rogueSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/roguechar.png", 
      16, 16, 32, 32);
  rogue.x = 8.f;
  rogue.y = 0.25f;
  rogue.z = 9.f;
  rogue.sprite = &rogueSprite;
  rogue.anim = &rogueAnim;
  rogue.state = 0;
  rogue.framerate = 12;
  rogue.frameTimer = 0.0f;

  wizardSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/wizardchar.png", 
      16, 16, 32, 32);
  wizard.x = 8.f;
  wizard.y = 0.25f;
  wizard.z = 7.f;
  wizard.sprite = &wizardSprite;
  wizard.anim = &wizardAnim;
  wizard.state = 0;
  wizard.framerate = 12;
  wizard.frameTimer = 0.0f;

  knightSprite = createAnimatedSprite(VBO, VBO, 
      12.f, 12.f, 12.f, 
      "res/knightchar.png", 
      16, 16, 32, 32);
  knight.x = 9.f;
  knight.y = 0.25f;
  knight.z = 8.f;
  knight.sprite = &knightSprite;
  knight.anim = &knightAnim;
  knight.state = 0;
  knight.framerate = 12;
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

  // load song data
  // If song directory + name is over 2000 characters I dunno what to tell you.
  char songFile[2000];
  strcpy(songFile, "res/songs/");
  strcat(songFile, selectedSong);
  strcat(songFile, ".txt");
  readSong(songFile, aNotes, &totalScore);
  resetSong();
  // resize viewport on load
  glViewport(0, 0, w, h);
  gltViewport(w, h);
  windowWidth = w;
  windowHeight = h;
  glUseProgram(shader);
  setProjection(shader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(lightingShader);
  setProjection(lightingShader, "proj", activeCamera, true, windowWidth, windowHeight);
  glUseProgram(shader);
}

void SetupLighting() {
  lightCubes[0] = createCubeLight(VBO, 6.0f, 1.5f, 11.0f);
  lightCubes[1] = createCubeLight(VBO, 1.0f, 1.5f, 11.0f);
  lightCubes[2] = createCubeLight(VBO, 10.0f, 1.5f, 11.0f);
  lightCubes[3] = createCubeLight(VBO, 10.0f, 1.5f, 4.0f);

  vec3 lDir = {-0.2f, -1.0f, -0.3f};
  vec3 lAmb = {0.1f, 0.1f, 0.1f};
  vec3 lDiff = {0.2f, 0.2f, 0.2f};
  setDirectionalLight(lDir, lDiff, lAmb, shader);
  for (int i = 0; i < 4; i++) {
    vec3 lPos = { lightCubes[i].posX, lightCubes[i].posY, lightCubes[i].posZ };
    vec3 diff;
    if (i == 0) {
      vec3 diff = {0.0f, 0.0f, 1.f};
      setPointLight(i, lPos, diff, shader);
    } else if (i == 1) {
      vec3 diff = {0.0f, 1.0f, 0.f};
      setPointLight(i, lPos, diff, shader);
    }
    else if (i == 2) {
      vec3 diff = { 1.f, 0.0f, 0.0f };
      setPointLight(i, lPos, diff, shader);
    }
    else {
      vec3 diff = { 0.1f, 0.2f, 0.1f };
      setPointLight(i, lPos, diff, shader);
    }
  }
}
void GameUpdate(float deltaTime) {
    dt = deltaTime;
    if (songStarted) {
      songTime += deltaTime;
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_VERTEX_ARRAY, VBO);
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, activeCamera->position);

    //processCameraMovement(activeCamera, &inputs, true, up);
    moveCamera();

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
      if (tiles[i] != 34 && tiles[i] != 41) {
        renderTile(tiles[i], row, col, tile, view, tilesTexture, VBO, shader);
      } else {
        resetTileTexCoords(planeData, VBO);
       if (tiles[i] == 34) {
         renderWall(tiles[i], row, col, tile, view, wallTexture, blackTexture, wallTexture, VBO, shader);
       } else {
         renderWall(tiles[i], row, col, tile, view, wallTexture, blackTexture, wallFlagTexture,VBO, shader);
       }
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
    glDrawArrays(GL_TRIANGLES, 0, 6);

    mat4x4_identity(model);
    mat4x4_translate_in_place(model, bard.x + 0.1f, bard.y - 0.1f, bard.z);
    mat4x4_scale_aniso(model, model, 0.25f, 0.35f, 0.35f);
    mat4x4_rotate_Y(model, model, degToRad(90.0f));
    if (guitarRotateUp) {
      mat4x4_rotate_Z(model, model, degToRad(15.0f));
    } else {
      mat4x4_rotate_Z(model, model, degToRad(-15.0f));
    }
    modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, guitarSprite.texture);
    Animate(&guitar, guitarAnim, true, deltaTime, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    mat4x4_identity(model);
    mat4x4_translate_in_place(model, rogue.x, rogue.y, rogue.z);
    mat4x4_scale_aniso(model, model, 0.5f, 0.5f, 0.5f);
    mat4x4_rotate_Y(model, model, degToRad(90.f));
    modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rogueSprite.texture);
    Animate(&rogue, rogueAnim, true, deltaTime, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    mat4x4_identity(model);
    mat4x4_translate_in_place(model, wizard.x, wizard.y, wizard.z);
    mat4x4_scale_aniso(model, model, 0.5f, 0.5f, 0.5f);
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
    mat4x4_scale_aniso(model, model, 0.5f, 0.5f, 0.5f);
    mat4x4_rotate_Y(model, model, degToRad(90.0f));
    modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, knightSprite.texture);
    Animate(&knight, knightAnim, true, deltaTime, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(uiShader);
    if (!menuOpen) {
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
          blueNoteTex,
          blueNoteOctTex,
          redNoteTex,
          redNoteOctTex,
          greenNoteTex,
          greenNoteOctTex,
          nextNoteIndex,
          setNextNote,
          windowWidth,
          windowHeight,
          setSongEnded);

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
    }
      // rendering note effect on key

    gltBeginDraw();
    char str[128];

    if (!songStarted && !menuOpen) {
      if (showScore) {
        gltSetText(text, "Score:");
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 450.0f, 4.0f, GLT_CENTER, GLT_TOP);

        gltSetText(text, itoa(lastScore, str, 10));
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 350.0f, 5.0f, GLT_CENTER, GLT_TOP);
        // letter grade
        float perc = (float)lastScore / (float)totalScore;
        float gradeScale = 10.0f;

        if (perc == 1.0) {
          gltSetText(text, "S");
          gltColor(0.922f, 0.8f, 0.122f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } else if (perc > 0.95f) {
          gltSetText(text, "A+");
          gltColor(0.122f, 0.627f, 0.922f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } 
        else if (perc > 0.9f) {
          gltSetText(text, "A");
          gltColor(0.122f, 0.627f, 0.922f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } 
        else if (perc > 0.85f) {
          gltSetText(text, "B+");
          gltColor(0.122f, 0.922f, 0.435f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        }
        else if (perc > 0.8f) {
          gltSetText(text, "B");
          gltColor(0.122f, 0.922f, 0.435f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } 
        else if (perc > 0.75f) {
          gltSetText(text, "C+");
          gltColor(0.922f, 0.122f, 0.847f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } 
        else if (perc > 0.7f) {
          gltSetText(text, "C");
          gltColor(0.922f, 0.122f, 0.847f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } 
        else if (perc > 0.65f) {
          gltSetText(text, "D+");
          gltColor(0.922f, 0.475f, 0.122f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } 
        else if (perc > 0.6f) {
          gltSetText(text, "D");
          gltColor(0.922f, 0.475f, 0.122f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        } else {
          gltSetText(text, "F");
          gltColor(1.0f, 0.0f, 0.0f, 1.0f);
          gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2 - 200.0f, gradeScale, GLT_CENTER, GLT_TOP);
        }
      }
      gltSetText(text, "Hit Space to start/stop playback!");
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2DAligned(text, (float)windowWidth / 2, (float)windowHeight / 2, 4.0f, GLT_CENTER, GLT_TOP);
    } else if (songStarted && !menuOpen) {
      gltSetText(text, itoa(currentScore, str, 10));
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(text, 10.f, 0.5f, 4.0f);

      gltSetText(text, "/");
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(text, 80.f, 0.5f, 4.0f);

      gltSetText(text, itoa(totalScore, str, 10));
      gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      gltDrawText2D(text, 120.f, 0.5f, 4.0f);
    }
    if (menuOpen) {
      // TITLE!
      gltSetText(text, "A Knight's Rest");
      gltColor(0.592f, 0.773f, 0.89f, 1.0f);
      gltDrawText2D(text, 120.f, 60.f, 10.0f);

      gltSetText(text, "Start");
      if (selMenuIndex == 0) {
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      } else {
        gltColor(0.5f, 0.5f, 0.5f, 1.0f);
      }
      gltDrawText2D(text, 120.f, 320.f, 4.0f);

      gltSetText(text, "Select Song");
      if (selMenuIndex == 1) {
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      } else {
        gltColor(0.5f, 0.5f, 0.5f, 1.0f);
      }
      gltDrawText2D(text, 120.f, 400.f, 4.0f);

      gltSetText(text, "How to play");
      if (selMenuIndex == 2) {
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      } else {
        gltColor(0.5f, 0.5f, 0.5f, 1.0f);
      }
      gltDrawText2D(text, 120.f, 480.f, 4.0f);

      if (fullScreen) {
        gltSetText(text, "Set Windowed");
      } else {
        gltSetText(text, "Set Fullscreen");
      }
      if (selMenuIndex == 3) {
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      } else {
        gltColor(0.5f, 0.5f, 0.5f, 1.0f);
      }
      gltDrawText2D(text, 120.f, 560.f, 4.0f);

      gltSetText(text, "Exit");
      if (selMenuIndex == 4) {
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
      } else {
        gltColor(0.5f, 0.5f, 0.5f, 1.0f);
      }
      gltDrawText2D(text, 120.f, 640.f, 4.0f);
    }

    gltEndDraw();
    // testing menu
  //  vec3 mp = {0.0f, 0.0f, -0.2f};
  //  vec3 mr = {0.0f, 0.0f, 0.0f};
  //  vec3 sc = {1.0f, 1.0f, 1.0f};
  //  renderPlane(mp, mr, sc, VBO, blackTexture, uiShader);

}

void DeleteBuffers() {
  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);
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

//  mouseLook(xoffset, yoffset, activeCamera, dt);
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
      return;
    } 
 }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  bool guitarAngleChange = false;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    if (!menuOpen) {
      songStarted = false;
      resetSong();
      menuOpen = true;
    } else {
      menuOpen = false;
    }
  }
//  if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
//    resetSong();
//  }
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !menuOpen) {
    songStarted = !songStarted;
    if (!songStarted) {
      resetSong();
    }
  }
  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    if (mods == 1) {
      playString(1, "res/a4.wav", true);
    } else {
      playString(1, "res/a3.wav", false);
    }
    if (!guitarAngleChange) {
      guitarRotateUp = !guitarRotateUp;
      guitarAngleChange = true;
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
    if (!guitarAngleChange) {
      guitarRotateUp = !guitarRotateUp;
      guitarAngleChange = true;
    }

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
    if (!guitarAngleChange) {
      guitarRotateUp = !guitarRotateUp;
      guitarAngleChange = true;
    }

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
    if (!guitarAngleChange) {
      guitarRotateUp = !guitarRotateUp;
      guitarAngleChange = true;
    }

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
    if (!guitarAngleChange) {
      guitarRotateUp = !guitarRotateUp;
      guitarAngleChange = true;
    }

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
    if (!guitarAngleChange) {
      guitarRotateUp = !guitarRotateUp;
      guitarAngleChange = true;
    }

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

  // menu interactions here
  if (menuOpen) {
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
      if (selMenuIndex < 4) {
        selMenuIndex++;
      } else {
        selMenuIndex = 0;
      }
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
      if (selMenuIndex > 0) {
        selMenuIndex--;
      } else {
        selMenuIndex = 4;
      }
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS ||
        key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
      switch (selMenuIndex) {
        case 0:
          // start
          menuOpen = false;
          break;
        case 1:
          GameSetScreen(3);
          break;
        case 2:
          // how to play
          break;
        case 3:
          // toggle fullscreen/windowed
          if (glfwGetWindowMonitor(window) == NULL) {
            fullScreen = true;
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 
                glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
                glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
                GLFW_DONT_CARE);
          } else {
            fullScreen = false;
            glfwSetWindowMonitor(window, NULL, 0, 0, 1920, 1080, GLFW_DONT_CARE);
          }
          break;
        case 4:
          glfwSetWindowShouldClose(window, true);
          break;
      };
    }
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
    aNotes[i].render = true;
    aNotes[i].colour[0] = 1.0f;
    aNotes[i].colour[1] = 1.0f;
    aNotes[i].colour[2] = 1.0f;
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
  if (difference <= 0.2 && octave == aNotes[nextNoteIndex[string-1]].octave) {
    currentScore += aNotes[nextNoteIndex[string-1]].pointValue;
    aNotes[nextNoteIndex[string-1]].active = false;
    aNotes[nextNoteIndex[string-1]].render = false;
  } else {
    if ( aNotes[nextNoteIndex[string-1]].pointValue > 0) {
      aNotes[nextNoteIndex[string-1]].pointValue -= 1;
    }
  }
  setNextNote(string);
}

void moveCamera() {
  float speed = 0.0003f;
  vec3 camFront = {0.0f, 0.0f, -1.0f};
  vec3 nFront;
  vec3_scale(nFront, activeCamera->direction, speed);
  vec3_add(activeCamera->position, activeCamera->position, nFront);
  cameraMoveTicker++;
  if (cameraMoveTicker > cameraMoveTime) {
    cameraMoveTicker = 0;
    if (cameraIndex < CAMERA_COUNT - 1) {
      cameraIndex++;
    } else {
      cameraIndex = 0;
    }
    activeCamera->position[0] = cameraPositions[cameraIndex][0];
    activeCamera->position[1] = cameraPositions[cameraIndex][1];
    activeCamera->position[2] = cameraPositions[cameraIndex][2];
    activeCamera->yaw = cameraRotations[cameraIndex][0];
    activeCamera->pitch = cameraRotations[cameraIndex][1];
    setDirection(activeCamera);
  }
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    (void)pInput;
}

void setSongEnded() {
  lastScore = currentScore;
  showScore = true;
  songStarted = false;
  resetSong();
}


