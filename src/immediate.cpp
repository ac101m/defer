// Standard
#include <iostream>

// External
#include <GLT/Window.hpp>
#include <optparse.hpp>

// This project
#include <mesh.hpp>


// Add command line optioons to the option parser
void AddOptions(OptionParser& opt) {
  opt.Add(Option("displayx", 'x', ARG_TYPE_INT,
                 "Set width of the render area",
                 {"1024"}));
  opt.Add(Option("displayy", 'y', ARG_TYPE_INT,
                 "Set height of the render area",
                 {"768"}));
  opt.Add(Option("lights", 'l', ARG_TYPE_INT,
                 "Set number of lights",
                 {"64"}));
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
  window.camera.SetPos(0, 0, -2);
  window.EnableFpsCounter();

  // Build shader program
  GLT::ShaderProgram shader = GLT::ShaderProgram({
    GLT::Shader(GL_VERTEX_SHADER, "shaders/iVert.glsl"),
    GLT::Shader(GL_FRAGMENT_SHADER, "shaders/iFrag.glsl")});

  // Create some lights
  std::vector<glm::mat2x3> lights(2);
  lights[0] = {glm::vec3(0, 0, 1), glm::vec3(0.5, 0, 0)};
  lights[1] = {glm::vec3(0, 0, -1), glm::vec3(0, 0, 0.5)};
  // Mesh positioning
  std::vector<glm::vec3> meshPositions;
  glm::uvec3 gridSize = glm::uvec3(8, 8, 8);
  glm::vec3 gridStep = glm::vec3(1.5, 1.5, 1.5);
  for(unsigned i = 0; i < gridSize.x; i++) {
    for(unsigned j = 0; j < gridSize.y; j++) {
      for(unsigned k = 0; k < gridSize.z; k++) {
        glm::vec3 index = glm::vec3(i, j, k);
        meshPositions.push_back(index * gridStep);
      }
    }
  }

  // Set lighting uniforms
  shader.GetUniform("lights[0]").SetFMat2x3(lights.data(), lights.size());
  shader.GetUniform("lightCount").SetI1(lights.size());

  // Create test mesh
  GLT::Mesh cubeMesh = GenCubeMesh();

  // Input sensitifity stuff
  float rotateSpeed = 1.0f;
  float moveSpeed = 1.0f;
  float mouseSensitivity = 0.003f;

  // Camera movement
  float dFwd, dRight, dUp, dr;

  // Main render loop
  while(!window.ShouldClose()) {

    // get current time
    float dt = window.GetTimeDelta();

    // Cursor capture control
    glm::vec2 cursorDelta = window.GetCursorDelta() * mouseSensitivity;
    if(window.KeyPressed(GLFW_KEY_ESCAPE)) window.FreeCursor();
    if(window.KeyPressed(GLFW_KEY_M)) window.CaptureCursor();

    // Camera translation & rotation
    dr = dFwd = dRight = dUp = 0.0f;
    if(window.KeyPressed(GLFW_KEY_W)) dFwd += (dt * moveSpeed);
    if(window.KeyPressed(GLFW_KEY_S)) dFwd -= (dt * moveSpeed);
    if(window.KeyPressed(GLFW_KEY_A)) dRight += (dt * moveSpeed);
    if(window.KeyPressed(GLFW_KEY_D)) dRight -= (dt * moveSpeed);
    if(window.KeyPressed(GLFW_KEY_SPACE)) dUp += (dt * moveSpeed);
    if(window.KeyPressed(GLFW_KEY_C)) dUp -= (dt * moveSpeed);
    if(window.KeyPressed(GLFW_KEY_E)) dr += (dt * rotateSpeed);
    if(window.KeyPressed(GLFW_KEY_Q)) dr -= (dt * rotateSpeed);

    // Update camera
    window.camera.Move(dRight, dUp, dFwd);
    window.camera.MoveLook(-cursorDelta.x, cursorDelta.y, dr);

    // Compute mesh transform
    glm::mat4 mRot = glm::rotate(
      glm::mat4(1.0f),
      (float)window.GetTime() / 3,
      glm::vec3(0, 1, 0));

    // Draw the mesh
    for(unsigned i = 0; i < meshPositions.size(); i++) {
      glm::mat4 m = glm::mat4(1.0);
      m = glm::translate(m, meshPositions[i]) * mRot;
      cubeMesh.Draw(window.camera, shader, m);
    }

    // Update the stuff
    window.Refresh();
  }

  return 0;
}
