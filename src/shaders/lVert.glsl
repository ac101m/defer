#version 330 core

// Vertex position and UV only
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 vertUV;

// Outputs from the vertex shader
out vec2 fragUV;


void main() {
  gl_Position = vec4(vertPos, 1.0);
  fragUV = vertUV;
}
