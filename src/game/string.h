#include "../../dep/linmath.h"
#include "note.h"
#ifndef H_STRING
#define H_STRING
void RenderString(unsigned int VBO, unsigned int shader, unsigned int texture, float y, int windowWidth);
void RenderNote(unsigned int VBO, unsigned int shader, unsigned int texture, Note *note, float deltaTime);
#endif
