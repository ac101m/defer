#version 330 core

// Vertex attribute inputs
layout (location = 0) in vec3 vertPosMs;
layout (location = 1) in vec2 vertUV;
layout (location = 2) in vec3 vertNmlMs;
layout (location = 3) in vec3 vertTanMs;
layout (location = 4) in vec3 vertBitanMs;

// Uniform matrices
uniform mat4 mMx;
uniform mat4 vMx;
uniform mat4 mvpMx;

// Outputs from the vertex shader
out VS_OUT {
  vec2 fragUV;
  vec3 vertPosWs;
  vec3 vertNmlWs;
  mat3 tbnMx;
} vsOut;


void main() {

  // Pass through UVs
  vsOut.fragUV = vertUV;

  // Vertex position in world space, for lighting purposes
  vsOut.vertPosWs = vec3(mMx * vec4(vertPosMs, 1.0));

  // Vertex normal in world space, also for lighting
  vsOut.vertNmlWs = vec3(mMx * vec4(vertNmlMs, 1.0));

  // Translate tangent space basis into world space
  vec3 T = normalize(vec3(mMx * vec4(vertTanMs, 0.0)));
  vec3 B = normalize(vec3(mMx * vec4(vertBitanMs, 0.0)));
  vec3 N = normalize(vec3(mMx * vec4(vertNmlMs, 0.0)));

  // Construct the TBN matrix and pass to fragment shader
  vsOut.tbnMx = mat3(T, B, N);

  // Fragment position
  gl_Position = mvpMx * vec4(vertPosMs, 1.0);
}
