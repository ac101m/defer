#ifndef DEFER_SCREENQUAD_INCLUDED
#define DEFER_SCREENQUAD_INCLUDED


// Standard
#include <vector>

// External
#include <GLT/GL/Texture.hpp>

// This project
#include <mesh.hpp>


class ScreenQuad : public GLT::Mesh {
public:

  // Constructor, construct the base mesh
  ScreenQuad(std::vector<GLT::Texture> textures) :
    GLT::Mesh(GenFullscreenQuadMesh(textures)) {
  }

  // Draw the fullscreen quad
  void Draw(GLT::Camera& camera, GLT::ShaderProgram& shader, glm::mat4& m) {

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


  // Simplified draw method
  void Draw(GLT::ShaderProgram& shader) {
    GLT::Camera tmpCam = GLT::Camera();
    glm::mat4 tmpMx = glm::mat4(1.0);
    this->Draw(tmpCam, shader, tmpMx);
  }
};


#endif // DEFER_SCREENQUAD_INCLUDED
