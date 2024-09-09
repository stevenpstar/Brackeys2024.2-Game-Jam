#ifndef H_SHADER
#define H_SHADER
#define INFOLOG_SIZE 512
unsigned int createShaderObject(const char* filePath, int type);
unsigned int createProgram(unsigned int vShader, unsigned int fShader);
unsigned int createShader(const char* vShaderPath, const char* fShaderPath);
#endif
