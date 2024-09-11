#include "../../dep/glad/glad.h"
#include "../../dep/linmath.h"
#include "../engine/camera.h"
#include "note.h"

void RenderString(unsigned int VBO, unsigned int shader, unsigned int texture, float y) {
  float data[30] = {
    -1.f, -0.005f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
     1.f, -0.005f, -0.0f,  1.f, 0.f, // bottom right
     1.f,  0.005f, -0.0f,  1.f, 1.f, // top right
     1.f,  0.005f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
    -1.f,  0.005f, -0.0f,  0.f, 1.f, // top left
    -1.f, -0.005f, -0.0f,  0.f, 0.f, // bottom left duplicate     
  };

  float playInd[30] = {
    -0.8f, -0.1f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
     -0.75f, -0.1f, -0.0f,  1.f, 0.f, // bottom right
     -0.75f,  0.1f, -0.0f,  1.f, 1.f, // top right
     -0.75f,  0.1f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
    -0.8f,  0.1f, -0.0f,  0.f, 1.f, // top left
    -0.8f, -0.1f, -0.0f,  0.f, 0.f, // bottom left duplicate     
  };

  glUseProgram(shader);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  mat4x4 model;
  mat4x4_identity(model);
  mat4x4_translate_in_place(model, 0.0f, y, 0.0f);
  unsigned int modelLoc = glGetUniformLocation(shader, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);

  vec3 colour = {1.0f, 1.0f, 1.0f};
  glUniform3f(glGetUniformLocation(shader, "col"), colour[0], colour[1], colour[2]);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // render play point timer indicator
  //
//  glBindBuffer(GL_ARRAY_BUFFER, VBO);
//  glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), playInd, GL_STATIC_DRAW);
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//  glEnableVertexAttribArray(1);
//  mat4x4_identity(model);
//  mat4x4_translate_in_place(model, 0.0f, -0.5f, 0.0f);
//  modelLoc = glGetUniformLocation(shader, "model");
//  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
//
//  glActiveTexture(GL_TEXTURE0);
//  glBindTexture(GL_TEXTURE_2D, texture);
//  glDrawArrays(GL_TRIANGLES, 0, 6);

}

void RenderNote(unsigned int VBO, unsigned int shader, unsigned int texture, Note *note, float songTime) {
  float data[30] = {
    -0.1f, -0.1f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
     0.1f, -0.1f, -0.0f,  1.f, 0.f, // bottom right
     0.1f,  0.1f, -0.0f,  1.f, 1.f, // top right
     0.1f,  0.1f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
    -0.1f,  0.1f, -0.0f,  0.f, 1.f, // top left
    -0.1f, -0.1f, -0.0f,  0.f, 0.f, // bottom left duplicate     
  };
  glUseProgram(shader);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  mat4x4 model;
  mat4x4_identity(model);
  mat4x4_translate_in_place(model, note->position[0], -0.5f, 0.0f);
  unsigned int modelLoc = glGetUniformLocation(shader, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Update note position
  note->position[0] = note->time - songTime;
}
