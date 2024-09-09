#include "../../dep/glad/glad.h"
#include "texture.h"
#include <math.h>
#include <stdio.h>
#include "primitives.h"
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
      .currentFrame = 14,
      .frameTime = 0.0f
    };

  SetFrame(&animSprite, animSprite.currentFrame, VBO, false);

  unsigned int t = loadTexture(tex);
  animSprite.texture = t;

  return animSprite;
}

void SetFrame(Sprite *sprite, int frame, unsigned int VBO, bool flipHorizontal) {
  float fw = sprite->frameWidth * sprite->currentFrame;
  int rowNum = floor(fw / sprite->texWidth);
  int colNum = (fw - (rowNum * sprite->texWidth)) / sprite->frameWidth;
 // printf("rowNum: %d\n", rowNum);
 // printf("colNum: %d\n", colNum);
  float row = ((float)rowNum * sprite->frameHeight) / sprite->texHeight;
  float col = (fw - (rowNum * sprite->frameWidth));// / texWidth;
  float normWidth = (float)sprite->frameWidth / (float)sprite->texWidth;
  float normHeight = (float)sprite->frameHeight / (float)sprite->texHeight;

  float topY = 1.0 - normHeight * rowNum;
  float bottomY = 1.0 - (normHeight * rowNum) - normHeight;
  float left = 0.0 + normWidth * colNum;
  float right = 0.0 + (normWidth * colNum) + normWidth;
  //printf("top: %f, bottom: %f, left: %f, right: %f\n", topY, bottomY, left, right);
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(sprite->data), sprite->data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
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

