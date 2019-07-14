#version 330 core

// G-buffer elements
uniform sampler2D texture0;   // Position
uniform sampler2D texture1;   // Normals
uniform sampler2D texture2;   // Colours + specular

// Texture coordinates from
in vec2 fragUV;

// Misc uniforms
uniform float ambientLighting = 0.05;

// Bunch o lights
uniform int lightCount = 0;
uniform mat2x3 lights[64];

// Outputs
out vec4 fragColourOut;


void main() {

  // Do we need to consider this fragment?
  float doNotForgetMe = texture(texture2, fragUV).a;
  if(doNotForgetMe == 0.0) discard;

  // Get G-buffer components
  vec3 fragPosWs = texture(texture0, fragUV).rgb;
  vec3 fragNmlWs = texture(texture1, fragUV).rgb;
  vec3 fragColour = texture(texture2, fragUV).rgb;

  // Compute point light contribution
  vec3 pointLighting = vec3(0.0);
  for(int i = 0; i < lightCount; i++) {
    vec3 l = lights[i][0] - fragPosWs;
    float dist = length(l);
    float cosTheta = clamp(dot(fragNmlWs, l), 0.0, 1.0);
    pointLighting += (lights[i][1] * cosTheta) / (dist * dist);
  }

  // Add up lighting contributions
  vec3 fragLighting = pointLighting + vec3(ambientLighting);
  vec3 colour = fragColour * fragLighting;
  fragColourOut = vec4(colour, 1.0);
}
