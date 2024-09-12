#include <stdbool.h>
#include "../../dep/linmath.h"
#ifndef H_NOTE
#define H_NOTE
typedef struct Note {
  float data[30];
  vec2 position;
  float time;
  bool active;
} Note;

typedef struct ANote {
  int string;
  float time;
  float duration;
  vec3 colour;
  bool active;
} ANote;

void initNote(Note *note, int string, float time, float duration);
void renderNotes(ANote notes[30],
    int notePoolCount,
    float songTime,
    unsigned int VBO,
    unsigned int shader,
    unsigned int blueTex,
    unsigned int blueOctTex,
    unsigned int redTex,
    unsigned int redOctTex,
    unsigned int greenTex,
    unsigned int greenOctTex,
    int nextNoteIndex[3],
    void (*setNextNote)(int),
    int windowWidth,
    int windowHeight);
#endif
