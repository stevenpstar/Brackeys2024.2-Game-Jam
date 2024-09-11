#include "stdlib.h"
#include <stdio.h>
#include <string.h>
#include "note.h"
// max notes is 1024 for now
void readSong(const char *filePath, ANote *aNotes, int *totalScore) {
  FILE *fp;
  fp = fopen(filePath, "r");
  char s[1024];
  int noteCount = 0; // note count
  if (fp == NULL) {
    printf("Song %s not found\n", filePath);
  } else {
    while (fgets(s, sizeof s, fp) != NULL) {

//      printf("%d: %s", ++noteCount, s);
      // get time here
      char *token = strtok(s, ",");
      if (token != NULL) {
        aNotes[noteCount].time = atof(token);
        aNotes[noteCount].colour[0] = 0.0f;
        aNotes[noteCount].colour[1] = 0.0f;
        aNotes[noteCount].colour[2] = 0.0f;
        aNotes[noteCount].active = true;

      } 
      token = strtok(NULL, ",");
      if (token != NULL) {
        printf("Token tho: %s\n", token);
        printf("Integer token %d\n", (int)atol(token));
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
      aNotes[noteCount].time = -100.0f;
      aNotes[noteCount].string = -1;
      // -1 will denote end of song for now
    }
    *totalScore = noteCount * 2;
  }
  fclose(fp);
}

