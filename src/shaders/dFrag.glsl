#version 330 core

// Output buffers
layout (location = 0) out vec3 gPosWs;
layout (location = 1) out vec3 gNmlWs;
layout (location = 2) out vec4 gAlbedo;

// Textures
uniform sampler2D texture0;   // Colour
uniform sampler2D texture1;   // Normal
uniform sampler2D texture2;   // Roughness

// Inputs from vertex shader
in VS_OUT {
  vec2 fragUV;
  vec3 vertPosWs;
  vec3 vertNmlWs;
  mat3 tbnMx;
} fsIn;


void main() {
  vec3 texColour = texture(texture0, fsIn.fragUV).rgb;
  vec3 texNormal = texture(texture1, fsIn.fragUV).rgb;
  vec3 texRoughness = texture(texture2, fsIn.fragUV).rgb;

  // Get our fragment normal from the
  vec3 fragNmlWs = normalize((texNormal * 2.0) - 1.0);
  fragNmlWs = normalize(fsIn.tbnMx * fragNmlWs);

  // Write values to the G buffer
  gPosWs = fsIn.vertPosWs;
  gNmlWs = fragNmlWs;
  gAlbedo.rgb = texColour.rgb;
  gAlbedo.a = 1.0;
}
