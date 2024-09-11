#include "../../dep/glad/glad.h"
#include <stdbool.h>
#include "../engine/primitives.h"
void renderKey(UISprite *key,
    bool flag,
    unsigned int VBO,
    unsigned int texture,
    unsigned int shader,
    int windowWidth, int windowHeight) {

    mat4x4 model;
    mat4x4_identity(model);
    mat4x4_translate_in_place(model,
        key->posX + 0.025f,
        key->posY - 0.035f, -0.1f);
    mat4x4_scale_aniso(model, model, 0.1f * (float)windowHeight/(float)windowWidth, 0.1f, 0.0f);
    key->currentFrame = flag ? 1 : 0;
    SetFrameUI(key, 0, VBO, false);
    unsigned int modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model);
    glUniform3f(glGetUniformLocation(shader, "col"), 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, key->texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
