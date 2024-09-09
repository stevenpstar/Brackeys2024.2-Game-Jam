#include "texture.h"
#include "../../dep/glad/glad.h"
#include "../../dep/stb_image.h"
unsigned int loadTexture(const char *path) {
  stbi_set_flip_vertically_on_load(1);
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // texture settings, may separate this out to be configurable later.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  int width, height, channels;
  unsigned char *imageData = stbi_load(path, &width, &height, &channels, 0);
  if (imageData) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
   // glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture %s\n", path);
  }

  stbi_image_free(imageData);

  return texture;
}

unsigned int loadTextureRGB(const char *path) {
  stbi_set_flip_vertically_on_load(1);
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // texture settings, may separate this out to be configurable later.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  int width, height, channels;
  unsigned char *imageData = stbi_load(path, &width, &height, &channels, 3);
  if (imageData) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
   // glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to load texture %s\n", path);
  }
 // for (int i = 0;i<(32*32*3);i+=3) {
 //   printf("r: %d, g: %d, b: %d\n", imageData[i], imageData[i+1], imageData[i+2]);
 // }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  GLubyte pixels[32*32*3] = {0};
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, &pixels);
  glGetError();
  //print pixel data
  for (int i = 0;i<(32*32*3);i+=3) {
    if (pixels[i] != 0 || pixels[i+1] != 0 || pixels[i+2] != 0) {
   //   printf("r: %d, g: %d, b: %d\n", pixels[i], pixels[i+1], pixels[i+2]);
    }
  }

  stbi_image_free(imageData);

  return texture;
}
