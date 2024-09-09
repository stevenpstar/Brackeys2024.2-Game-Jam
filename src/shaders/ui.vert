#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Colour;

uniform mat4 model;
uniform vec3 col;

void main() {
  TexCoord = aTexCoord;
  Colour = col;
  gl_Position = model * vec4(aPos, 1.0);
}
