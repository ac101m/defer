#version 330 core

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

// Misc uniforms
uniform float ambientLighting = 0.05;

// Bunch o lights
uniform int lightCount = 0;
uniform mat2x3 lights[64];

// Outputs
out vec4 fragColour;


void main() {
  vec3 texColour = texture(texture0, fsIn.fragUV).rgb;
  vec3 texNormal = texture(texture1, fsIn.fragUV).rgb;

  // Get our fragment normal from the
  vec3 fragNmlWs = normalize((texNormal * 2.0) - 1.0);
  fragNmlWs = normalize(fsIn.tbnMx * fragNmlWs);

  // Compute point lighting contribution
  vec3 pointLighting = vec3(0.0);
  for(int i = 0; i < lightCount; i++) {
    vec3 l = lights[i][0] - fsIn.vertPosWs;
    float dist = length(l);
    float cosTheta = clamp(dot(fragNmlWs, l), 0.0, 1.0);
    pointLighting += (lights[i][1] * cosTheta) / (dist * dist);
  }

  // Add up lighting contributions
  vec3 fragLighting = pointLighting + vec3(ambientLighting);
  vec3 colour = texColour * fragLighting;
  fragColour = vec4(colour, 1.0);
}
