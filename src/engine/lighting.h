#include "../../dep/linmath.h"
#ifndef H_LIGHTING
#define H_LIGHTING
void setPointLight(int index, vec3 position, vec3 diffuse, unsigned int shader);
void setDirectionalLight(vec3 direction, vec3 diffuse, vec3 ambient, unsigned int shader);
#endif
