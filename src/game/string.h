#include "../../dep/linmath.h"
#include "note.h"
#ifndef H_STRING
#define H_STRING
void RenderString(unsigned int VBO, unsigned int shader, unsigned int texture, Camera *camera);
void RenderNote(unsigned int VBO, unsigned int shader, unsigned int texture, Camera *camera, Note *note, float deltaTime);
#endif
