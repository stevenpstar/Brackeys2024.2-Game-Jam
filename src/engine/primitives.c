#include "../../dep/glad/glad.h"
#include "texture.h"
#include <math.h>
#include <stdio.h>
#include "primitives.h"
#include "../../dep/linmath.h"
P_CUBE createCube(unsigned int VBO) {
  P_CUBE cube = {
    .posX = -1.f,
    .posY = 0.0f,
    .posZ = 0.0f,
    .vertices = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         },
  };

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube.vertices), cube.vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  return cube;
}

P_CUBE_LIGHT createCubeLight(unsigned int VBO, float x, float y, float z) {
  P_CUBE_LIGHT light = {
    .posX = x,
    .posY = y,
    .posZ = z,
    .vertices = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f,
    },
  };
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(light.vertices), light.vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
//  int32_t bsize = 0;
//  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);
//  if (bsize == 0) {
//    printf("Nothing in light cube array buffer\n");
//  }

  return light;
}

void setCubeLight(unsigned int VBO, float *vertices) {
    float verts[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f,
    };

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  int32_t bsize = 0;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);
  if (bsize == 0) {
    printf("Nothing in light cube array buffer\n");
  }
}

Sprite createAnimatedSprite(unsigned int VBO, unsigned int EBO,
    float x, float y, float z,
    const char *tex, int frameWidth, int frameHeight, int texWidth, int texHeight) {

    int rows = texHeight / frameHeight;
    int cols = texWidth / frameWidth;
    int frames = cols * rows;

    Sprite animSprite = {
      .posX = x,
      .posY = y,
      .posZ = z,
      .data = {
        -0.5f, -0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0 - 0.083333f,//0.0f, // bottom left
         0.5f, -0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.25f, 1.0 - 0.083333f, // bottom right
         0.5f,  0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.25f, 1.f, // top right
         0.5f,  0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.25f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
        -0.5f,  0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
        -0.5f, -0.5f, -0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0 - 0.083333f, // bottom left duplicate     
      },
      .indices = {
        0, 1, 3,
        1, 2, 3
      },
      .frameWidth = frameWidth,
      .frameHeight = frameHeight,
      .texWidth = texWidth,
      .texHeight = texHeight,
      .frameCount = frames,
      .currentFrame = 0,
      .frameTime = 0.0f
    };

  SetFrame(&animSprite, animSprite.currentFrame, VBO, false);

  unsigned int t = loadTexture(tex);
  animSprite.texture = t;

  return animSprite;
}

UISprite createAnimatedUI(unsigned int VBO, vec3 pos, const char *tex, int fWidth, int fHeight,
    int tWidth, int tHeight) {
       int rows = tHeight / fHeight;
    int cols = tWidth / fWidth;
    int frames = cols * rows;

    UISprite animUI = {
      .posX = pos[0],
      .posY = pos[1],
      .posZ = pos[2],
      .data = {
          -0.5f, -0.5f, -0.0f,  0.f, 0.f,//0.0f, // bottom left
           0.5f, -0.5f, -0.0f,  1.f, 0.f, // bottom right
           0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right
           0.5f,  0.5f, -0.0f,  1.f, 1.f, // top right duplicate (ignore and/or change to be same not sure)
          -0.5f,  0.5f, -0.0f,  0.f, 1.f, // top left
          -0.5f, -0.5f, -0.0f,  0.f, 0.f, // bottom left duplicate     
       },
      .frameWidth = fWidth,
      .frameHeight = fHeight,
      .texWidth = tWidth,
      .texHeight = tHeight,
      .frameCount = frames,
      .currentFrame = 0,
      .frameTime = 0.0f,
    };

    SetFrameUI(&animUI, animUI.currentFrame, VBO, false);
    unsigned int t = loadTexture(tex);
    animUI.texture = t;
    return animUI;
}

void SetFrame(Sprite *sprite, int frame, unsigned int VBO, bool flipHorizontal) {
  float fw = sprite->frameWidth * sprite->currentFrame;
  int rowNum = floor(fw / sprite->texWidth);
  int colNum = (fw - (rowNum * sprite->texWidth)) / sprite->frameWidth;

  float row = ((float)rowNum * sprite->frameHeight) / sprite->texHeight;
  float col = (fw - (rowNum * sprite->frameWidth));// / texWidth;
  float normWidth = (float)sprite->frameWidth / (float)sprite->texWidth;
  float normHeight = (float)sprite->frameHeight / (float)sprite->texHeight;

  float topY = 1.0 - normHeight * rowNum;
  float bottomY = 1.0 - (normHeight * rowNum) - normHeight;
  float left = 0.0 + normWidth * colNum;
  float right = 0.0 + (normWidth * colNum) + normWidth;
  if (flipHorizontal) {
    float temp = left;
    left = right;
    right = temp;
  }
  // bottom left
  sprite->data[6] = left; // done
  sprite->data[7] = bottomY;// done
  sprite->data[46] = left;
  sprite->data[47] = bottomY;

  sprite->data[14] = right; // done
  sprite->data[15] = bottomY; // done

  sprite->data[22] = right; // done
  sprite->data[23] = topY; // done
  sprite->data[30] = right; // done
  sprite->data[31] = topY; // done

  sprite->data[38] = left; // done
  sprite->data[39] = topY; // done

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), sprite->data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

// UI Sprites don't have normals (not effected by lighting, so texture coordinate
// indexes are different
void SetFrameUI(UISprite *sprite, int frame, unsigned int VBO, bool flipHorizontal) {
  float fw = sprite->frameWidth * sprite->currentFrame;
  int rowNum = floor(fw / sprite->texWidth);
  int colNum = (fw - (rowNum * sprite->texWidth)) / sprite->frameWidth;
  float row = ((float)rowNum * sprite->frameHeight) / sprite->texHeight;
  float col = (fw - (rowNum * sprite->frameWidth));// / texWidth;
  float normWidth = (float)sprite->frameWidth / (float)sprite->texWidth;
  float normHeight = (float)sprite->frameHeight / (float)sprite->texHeight;

  float topY = 1.0 - normHeight * rowNum;
  float bottomY = 1.0 - (normHeight * rowNum) - normHeight;
  float left = 0.0 + normWidth * colNum;
  float right = 0.0 + (normWidth * colNum) + normWidth;

  if (flipHorizontal) {
    float temp = left;
    left = right;
    right = temp;
  }

  sprite->data[3] = left;
  sprite->data[4] = bottomY;
  sprite->data[28] = left;
  sprite->data[29] = bottomY;

  sprite->data[8] = right;  
  sprite->data[9] = bottomY;

  sprite->data[13] = right; 
  sprite->data[14] = topY;
  sprite->data[18] = right;  
  sprite->data[19] = topY;

  sprite->data[23] = left;
  sprite->data[24] = topY; 

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), sprite->data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}


void updateCubeVBO(unsigned int VBO, P_CUBE *cube) {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube->vertices), cube->vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

