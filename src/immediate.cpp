// Standard
#include <iostream>

// External
#include <GLT/Window.hpp>
#include <optparse.hpp>


// Add command line optioons to the option parser
void AddOptions(OptionParser& opt) {
  opt.Add(Option("displayx", 'x', ARG_TYPE_INT,
                 "Set width of the render area",
                 {"1024"}));
  opt.Add(Option("displayy", 'y', ARG_TYPE_INT,
                 "Set height of the render area",
                 {"768"}));
}


// Mesh draw code
void GLT::Mesh::Draw(
  GLT::Camera& camera,
  GLT::ShaderProgram& shader,
  glm::mat4& m) {

  // Formulate MVP matrix
  glm::mat4 mvp = camera.GetProjMat() * camera.GetViewMat() * m;

  // Shader uniform setup
  shader.GetUniform("mMx").SetFMat4(&m);
  shader.GetUniform("vMx").SetFMat4(&camera.GetViewMat());
  shader.GetUniform("mvpMx").SetFMat4(&mvp);

  // Bind textures to texture units
  std::string name = "texture0";
  for(unsigned i = 0; i < this->textures.size(); i++) {
    name[7] = 48 + i;
    shader.SetTexture(i, name, this->textures[i]);
  }

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



int main(int argc, char **argv) {
  OptionParser opt(argc, argv, "Simple immediate renderer");
  AddOptions(opt);

  // Get display parameters and open a window
  int displayx = opt.Get("displayx");
  int displayy = opt.Get("displayy");
  GLT::Window window = GLT::Window(displayx, displayy, "immediate");

  // Main render loop
  while(!window.ShouldClose()) {
    window.Refresh();
  }

  return 0;
}
