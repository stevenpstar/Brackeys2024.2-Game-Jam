#include "../../dep/glad/glad.h"
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "file.h"

unsigned int createShaderObject(const char* filePath, const int type) {
  unsigned int shader;
  int success;
  char infoLog[INFOLOG_SIZE];

  FILE *fp;
  fp = fopen(filePath, "r");
  if (fp == NULL) {
    printf("FilePath: %s\n", filePath);
    printf("Shader file not found.\n");
    return -1;
  }
  const char *buffer = readFile(fp);
  fclose(fp);

  shader = glCreateShader(type);
  glShaderSource(shader, 1, &buffer, NULL);
  // We no longer need the shader string loaded from file, free the memory here (Created in readFile)
  free((char*)buffer);

  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, INFOLOG_SIZE, NULL, infoLog);
    printf("Shader failed to compile: %s\n", infoLog);
  } else {
    printf("%s successfully compiled\n", filePath);
  }
  
  return shader;
}

unsigned int createProgram(unsigned int vShader, unsigned int fShader) {
  unsigned int sProgram = glCreateProgram();
  int success;
  char infoLog[INFOLOG_SIZE];
  glAttachShader(sProgram, vShader);
  glAttachShader(sProgram, fShader);
  glLinkProgram(sProgram);
  glGetProgramiv(sProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(sProgram, INFOLOG_SIZE, NULL, infoLog);
    printf("Program failed to link shaders %s\n", infoLog);
  } else {
    printf("Linked shaders.\n");
  }

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  return sProgram;
}

unsigned int createShader(const char *vShaderPath, const char *fShaderPath) {
  unsigned int vShader, fShader, sProgram;
  vShader = createShaderObject(vShaderPath, GL_VERTEX_SHADER);
  fShader = createShaderObject(fShaderPath, GL_FRAGMENT_SHADER);
  sProgram = createProgram(vShader, fShader);
  return sProgram;
}

