#ifndef DEFER_MESH_INCLUDED
#define DEFER_MESH_INCLUDED


// External
#include <GLT/Mesh.hpp>

// This project
#include <frameBuffer.hpp>


// Generate full screen quad with textures
GLT::Mesh GenFullscreenQuadMesh(std::vector<GLT::Texture> textures) {
  std::vector<float> vertexData = {
    -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  0.0f,  1.0f,  0.0f};

  // Vertex list
  std::vector<GLT::vertex_t> vertices;
  for(unsigned i = 0; i < vertexData.size(); i+= 5) {
    GLT::vertex_t v;
    v.position = glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
    v.uv = glm::vec2(vertexData[i + 3], vertexData[i + 4]);
    vertices.push_back(v);
  }

  // Indices
  std::vector<unsigned> indices = {1, 0, 2, 3, 2, 0};

  // Build the mesh
  GLT::Mesh mesh(vertices, indices, textures);
  return mesh;
}


// Generates a cube test mesh with proper indices
GLT::Mesh GenCubeMesh(void) {
  std::vector<float> vertexData = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f};

  // Vertex list
  std::vector<GLT::vertex_t> vertices;
  for(unsigned i = 0; i < vertexData.size(); i += 5) {
    GLT::vertex_t v;
    v.position = glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
    v.uv = glm::vec2(vertexData[i + 3], vertexData[i + 4]);
    vertices.push_back(v);
  }

  // Compute all indices
  std::vector<unsigned> indices;
  for(unsigned i = 0; i < vertices.size(); i += 4) {
    indices.push_back(i);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
    indices.push_back(i + 2);
    indices.push_back(i + 3);
    indices.push_back(i);
  }

  // Load the side texture
  std::vector<GLT::Texture> textures = {
    GLT::Texture("textures/brownrock/colour.png"),
    GLT::Texture("textures/brownrock/normal.png"),
    GLT::Texture("textures/brownrock/roughness.png")};

  // Set sampling modes on the textures to linear
  for(unsigned i = 0; i < textures.size(); i++) {
    textures[i].Parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textures[i].Parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  // Package up into mesh and return
  GLT::Mesh mesh(vertices, indices, textures);
  mesh.AutoGenerateNormals();
  return mesh;
}


#endif // DEFER_MESH_INCLUDED
