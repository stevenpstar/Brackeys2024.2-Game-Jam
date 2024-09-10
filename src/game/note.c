#include "../../dep/glad/glad.h"
#include <stdio.h>
#include "note.h"
void initNote(Note *note, int string, float time, float duration) {}
void renderNotes(ANote *notes,
    int notePoolCount,
    float songTime,
    unsigned int VBO,
    unsigned int shader,
    unsigned int texture,
    int *nextNoteIndex,
    void (*setNextNote)(int),
    int windowWidth,
    int windowHeight) {

  float data[30] = {
    -0.5f, -0.5f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
     0.5f, -0.5f, -0.0f,  1.f, 0.f, // bottom right
     0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right
     0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
    -0.5f,  0.5f, -0.0f,  0.f, 1.f, // top left
    -0.5f, -0.5f, -0.0f,  0.f, 0.f, // bottom left duplicate     
  };

  vec2 position;
  glUseProgram(shader);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  vec3 colour = {0.0f, 0.0f, 1.0f};
  for (int i = 0; i < notePoolCount; i++) {
      if (notes[i].string < 0) {
        break;
      }
      position[0] = (notes[i].time - songTime) + 0.225f;
      position[1] = -0.35f - (0.1f * notes[i].string);
      if (songTime > notes[i].time + 1.0f && notes[i].active) {
        notes[i].active = false;
        if (nextNoteIndex[notes[i].string-1] == i) {
          setNextNote(notes[i].string);
        }
      }
      float aRatio = (float)windowWidth/(float)windowHeight;
      float hRatio = (float)windowHeight/(float)windowWidth;
      mat4x4 model;
      mat4x4_identity(model);
      mat4x4_translate_in_place(model, position[0], position[1], -0.1f);
      mat4x4_scale_aniso(model, model, 0.05f * (float)windowHeight/(float)windowWidth,
          0.05f, 1.f);
      unsigned int modelLoc = glGetUniformLocation(shader, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
      glUniform3f(glGetUniformLocation(shader, "col"), notes[i].colour[0], notes[i].colour[1], notes[i].colour[2]);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}
