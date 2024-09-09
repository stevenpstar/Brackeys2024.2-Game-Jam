#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Colour;
uniform sampler2D tex;

void main() {
//FragColor = texture(tex, TexCoord);
  FragColor = vec4(Colour, 1.0f);
}
