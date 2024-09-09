#include <stdbool.h>
#include "../../dep/linmath.h"
#ifndef H_NOTE
#define H_NOTE
typedef struct Note {
  float data[30];
  vec2 position;
  bool active;
} Note;

void initNote(Note *note, int string, float time, float duration);
#endif
