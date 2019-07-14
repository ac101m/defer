// Standard
#include <iostream>

// External
#include <GLT/Window.hpp>

// This project
#include <options.hpp>
#include <mesh.hpp>
#include <utils.hpp>



int main(int argc, char **argv) {
  OptionParser opt(argc, argv, "Simple immediate renderer");
  BuildOptionParser(opt);

  // Hint at majopr opengl version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Get display parameters and open a window
  int displayx = opt.Get("displayx");
  int displayy = opt.Get("displayy");
  GLT::Window window = GLT::Window(displayx, displayy, "immediate", glfwGetPrimaryMonitor());
  window.camera.SetPos(0, 0, -2);
  window.EnableFpsCounter();

  // Build shader program
  GLT::ShaderProgram shader = GLT::ShaderProgram({
    GLT::Shader(GL_VERTEX_SHADER, "shaders/iVert.glsl"),
    GLT::Shader(GL_FRAGMENT_SHADER, "shaders/iFrag.glsl")});

  // Mesh positioning
  std::vector<glm::vec3> meshPositions;
  std::vector<glm::vec3> meshRotationAxes;
  glm::uvec3 gridSize = glm::uvec3(8, 8, 8);
  glm::vec3 gridStep = glm::vec3(1.5, 1.5, 1.5);
  for(unsigned i = 0; i < gridSize.x; i++) {
    for(unsigned j = 0; j < gridSize.y; j++) {
      for(unsigned k = 0; k < gridSize.z; k++) {
        glm::vec3 index = glm::vec3(i, j, k);
        meshPositions.push_back(index * gridStep);
        meshRotationAxes.push_back(
          glm::normalize(RandVec3(glm::vec3(-1.0), glm::vec3(1.0))));
      }
    }
  }

  // Randomly distribute point lights
  glm::vec3 lightMin = glm::vec3(0.0);
  glm::vec3 lightMax = glm::vec3(1.0);
  glm::vec3 positionMin = -gridStep;
  glm::vec3 positionMax = glm::vec3(glm::vec3(gridSize) * gridStep);
  int lightCount = opt.Get("lights");
  std::vector<glm::mat2x3> lights;
  for(int i = 0; i < lightCount; i++) {
    glm::vec3 lightIntensity = RandVec3(lightMin, lightMax);
    glm::vec3 lightPosition = RandVec3(positionMin, positionMax);
    lights.push_back({lightPosition, lightIntensity});
  }

  // Set lighting uniforms
  shader.GetUniform("lights[0]").SetFMat2x3(lights.data(), lights.size());
  shader.GetUniform("lightCount").SetI1(lights.size());

  // Create test mesh
  GLT::Mesh cubeMesh = GenCubeMesh();

  // Input sensitifity stuff
  float rotateSpeed = 1.0f;
  float moveSpeed = 5.0f;
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

    // Draw the meshes
    for(unsigned i = 0; i < meshPositions.size(); i++) {

      // Build the model matrix
      glm::mat4 m = glm::mat4(1.0);
      m = glm::translate(m, meshPositions[i]);
      m = glm::rotate(m, (float)window.GetTime() / 3, meshRotationAxes[i]);

      // Draw the meshes
      cubeMesh.Draw(window.camera, shader, m);
    }

    // Update the stuff
    window.Refresh();
  }

  return 0;
}
