#include <stdbool.h>
#ifndef H_INPUT
#define H_INPUT
typedef struct KeyInput {
  int Key;
  bool Down;
} KeyInput;

typedef struct Inputs {
  KeyInput forward;
  KeyInput backwards;
  KeyInput left;
  KeyInput right;
} Inputs;

void setDefaults(Inputs *inputs);

#endif
