#ifndef DEFER_MESH_INCLUDED
#define DEFER_MESH_INCLUDED


// External
#include <GLT/Mesh.hpp>


// Mesh draw code
void GLT::Mesh::Draw(
  GLT::Camera& camera,
  GLT::ShaderProgram& shader,
  glm::mat4& m) {

  // Formulate MVP matrix
  glm::mat4 mvp = camera.GetProjMat() * camera.GetViewMat() * m;

  // Shader uniform setup
  shader.GetUniform("mMx").SetFMat4(&m);
  shader.GetUniform("mvpMx").SetFMat4(&mvp);

  // Bind textures to texture units
  std::string name = "texture0";
  for(unsigned i = 0; i < this->textures.size(); i++) {
    name[7] = 48 + i;
    shader.SetTexture(i, name, this->textures[i]);
  }

  // Use the shader in question
  shader.Use();

  // Draw the things
  this->vertexBuffer.Bind();
  glDrawElements(
    GL_TRIANGLES,
    this->vertexBuffer.GetIndexBufferLength(),
    GL_UNSIGNED_INT, 0);
  this->vertexBuffer.Unbind();

  // Set texture back to texture 0
  glActiveTexture(GL_TEXTURE0);
}


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
