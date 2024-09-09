#include "../../dep/glad/glad.h"
#include "../../dep/linmath.h"
#include "lighting.h"

void setPointLight(int index, vec3 position, vec3 diffuse, unsigned int shader) {
  char c = index + '0';
  int strIndex = 12;
  char pos[50] = "pointLights[0].position\0";
  char ambient[50] = "pointLights[0].ambient\0";
  char diff[50] = "pointLights[0].diffuse\0";
  char specular[50] = "pointLights[0].specular\0";
  char constant[50] = "pointLights[0].constant\0";
  char linear[50] = "pointLights[0].linear\0";
  char quadratic[50] = "pointLights[0].quadratic\0";
  if (index > 0) {
    pos[strIndex] = c;
    ambient[strIndex] = c;
    diff[strIndex] = c;
    specular[strIndex] = c;
    constant[strIndex] = c;
    linear[strIndex] = c;
    quadratic[strIndex] = c;
  }

  glUseProgram(shader);
  glUniform3f(glGetUniformLocation(shader, pos), 
      position[0], position[1], position[2]);
  glUniform3f(glGetUniformLocation(shader, ambient), 0.05f, 0.05f, 0.05f);
  glUniform3f(glGetUniformLocation(shader, diff), diffuse[0], diffuse[1], diffuse[2]);
  glUniform3f(glGetUniformLocation(shader, specular), 1.0f, 1.0f, 1.0f);
  glUniform1f(glGetUniformLocation(shader, constant), 1.0f);
  glUniform1f(glGetUniformLocation(shader, linear), 0.09f);
  glUniform1f(glGetUniformLocation(shader, quadratic), 0.032f);
}

void setDirectionalLight(vec3 direction, vec3 diffuse, vec3 ambient, unsigned int shader) {
  glUseProgram(shader);
  glUniform3f(glGetUniformLocation(shader, "dirLight.direction"), direction[0], direction[1], direction[2]); 
  glUniform3f(glGetUniformLocation(shader, "dirLight.ambient"), ambient[0], ambient[1], ambient[2]); 
  glUniform3f(glGetUniformLocation(shader,"dirLight.diffuse"), diffuse[0], diffuse[1], diffuse[2]);
  glUniform3f(glGetUniformLocation(shader, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
}
