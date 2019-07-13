#ifndef DEFER_UTILS_INCLUDED
#define DEFER_UTILS_INCLUDED


// Standard
#include <stdlib.h>


// Return random float between min and max
float RandFloat(float const min, float const max) {
  float r = (float)rand() / (float)RAND_MAX;
  return min + (r * (max - min));
}


// Return random vector with component values between min and max
glm::vec3 RandVec3(glm::vec3 const min, glm::vec3 const max) {
  glm::vec3 randVec;
  randVec.x = RandFloat(min.x, max.x);
  randVec.y = RandFloat(min.y, max.y);
  randVec.z = RandFloat(min.z, max.z);
  return randVec;
}


#endif // DEFER_MESH_INCLUDED
