#include <stdio.h>
#include <stdlib.h>
/* 
 * This function does not free the memory of the buffer 
 * after the file is loaded into it.
 */
char* readFile(FILE *fp) {
  char *buffer;
  long numBytes;
  fseek(fp, 0L, SEEK_END);
  numBytes = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  buffer = (char*)calloc(numBytes, sizeof(char));
  if (buffer == NULL) {
    printf("Memory error callocing buffer (readFile)\n");
    return buffer;
  }
  fread(buffer, sizeof(char), numBytes, fp);
  return buffer;
}

