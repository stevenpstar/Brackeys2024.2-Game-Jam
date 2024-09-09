#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include "note.h"
// max notes is 1024 for now
void readSong(const char *filePath, ANote *aNotes) {
  FILE *fp;
  fp = fopen(filePath, "r");
  char s[1024];
  char *token;
  int noteCount = 0; // note count
  if (fp == NULL) {
    printf("Song %s not found\n", filePath);
  } else {
    while (fgets(s, sizeof s, fp) != NULL) {
//      printf("%d: %s", ++noteCount, s);
      // get time here
      token = strtok(s, ",");
      if (token != NULL) {
        aNotes[noteCount].time = atof(token);
      }
      token = strtok(s, ",");
      if (token != NULL) {
        aNotes[noteCount].string = atol(token);
      }
      printf("aNote: %f, %d\n", aNotes[noteCount].time, aNotes[noteCount].string);
      if (noteCount >= 1024) {
        // emergency exit
        break;
      }
      noteCount++;
    }
    if (noteCount < 1024 - 1) {
      aNotes[noteCount].time = 0.0f;
      aNotes[noteCount].string = -1;
      // -1 will denote end of song for now
    }
  }
  fclose(fp);
}

