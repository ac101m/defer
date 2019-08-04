// Standard
#include <iostream>

// External
#include <GLT/Window.hpp>
#include <GLT/GL/FrameBuffer.hpp>

// This project
#include <options.hpp>
#include <mesh.hpp>
#include <utils.hpp>
#include <screenQuad.hpp>



int main(int argc, char **argv) {
  OptionParser opt(argc, argv, "Simple deferred renderer");
  BuildOptionParser(opt);

  // Hint at majopr opengl version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Get primary monitor for fullscreen if defined
  GLFWmonitor* mon = NULL;
  if(opt.Specified("fullscreen")) {
    mon = glfwGetPrimaryMonitor();
  }

  // Get display parameters and open a window
  int displayx = opt.Get("displayx");
  int displayy = opt.Get("displayy");
  GLT::Window window = GLT::Window(displayx, displayy, "deferred", mon);

  // Build g buffer shader program
  GLT::ShaderProgram gBufferShader = GLT::ShaderProgram({
    GLT::Shader(GL_VERTEX_SHADER, "shaders/iVert.glsl"),
    GLT::Shader(GL_FRAGMENT_SHADER, "shaders/dFrag.glsl")});

  // Build lighting shader program
  GLT::ShaderProgram lightingShader = GLT::ShaderProgram({
    GLT::Shader(GL_VERTEX_SHADER, "shaders/lVert.glsl"),
    GLT::Shader(GL_FRAGMENT_SHADER, "shaders/lFrag.glsl")});

  // Mesh positioning
  std::vector<glm::vec3> meshPositions;
  std::vector<glm::vec3> meshRotationAxes;
  glm::ivec3 gridStart = glm::uvec3(-3, -3, -3);
  glm::ivec3 gridEnd = glm::uvec3(3, 3, 3);
  glm::vec3 gridStep = glm::vec3(1.5, 1.5, 1.5);
  for(int i = gridStart.x; i <= gridEnd.x; i++) {
    for(int j = gridStart.y; j <= gridEnd.y; j++) {
      for(int k = gridStart.z; k <= gridEnd.z; k++) {
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
  glm::vec3 positionMin = glm::vec3(gridStart - glm::ivec3(1)) * gridStep;
  glm::vec3 positionMax = glm::vec3(gridEnd + glm::ivec3(1)) * gridStep;
  int lightCount = opt.Get("lights");
  std::vector<glm::mat2x3> lights;
  for(int i = 0; i < lightCount; i++) {
    glm::vec3 lightIntensity = RandVec3(lightMin, lightMax);
    glm::vec3 lightPosition = RandVec3(positionMin, positionMax);
    lights.push_back({lightPosition, lightIntensity});
  }

  // Create G-buffer color buffers
  std::shared_ptr<GLT::Texture2D> gPosition(new GLT::Texture2D(
    0, GL_RGB16F, displayx, displayy, GL_RGB, GL_FLOAT, 0));
  std::shared_ptr<GLT::Texture2D> gNormal(new GLT::Texture2D(
    0, GL_RGB16F, displayx, displayy, GL_RGB, GL_FLOAT, 0));
  std::shared_ptr<GLT::Texture2D> gAlbedo(new GLT::Texture2D(
    0, GL_RGBA, displayx, displayy, GL_RGBA, GL_UNSIGNED_BYTE, 0));

  // Create renderbuffer to serve as depth buffer
  GLT::RenderBuffer gDepth = GLT::RenderBuffer(
    GL_DEPTH_COMPONENT, displayx, displayy);

  // Create G-buffer
  GLT::FrameBuffer gBuffer;
  gBuffer.AttachTexture2D(*gPosition);
  gBuffer.AttachTexture2D(*gNormal);
  gBuffer.AttachTexture2D(*gAlbedo);
  gBuffer.AttachRenderBuffer(gDepth, GL_DEPTH_ATTACHMENT);

  // Create fullscreen quad for g-buffer rendering
  ScreenQuad frameMesh({gPosition, gNormal, gAlbedo});

  // Set lighting uniforms in lighting shader
  lightingShader.GetUniform("lights[0]").SetFMat2x3(lights.data(), lights.size());
  lightingShader.GetUniform("lightCount").SetI1(lights.size());

  // Create test mesh
  GLT::Mesh cubeMesh = GenCubeMesh();

  // Main render loop
  while(!window.ShouldClose()) {

    // Place camera away from origin
    window.camera.SetPos(0, 0, -18);

    // Rotate camera around the origin
    window.camera.GetViewMat() = glm::rotate(
      window.camera.GetViewMat(),
      (float)window.GetTime() / 6,
      glm::vec3(0, 1, 0));

    // Draw meshes to g buffer
    gBuffer.Bind(GL_DRAW_FRAMEBUFFER);
    gBuffer.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    for(unsigned i = 0; i < meshPositions.size(); i++) {

      // Build the model matrix
      glm::mat4 m = glm::mat4(1.0);
      m = glm::translate(m, meshPositions[i]);
      m = glm::rotate(m, (float)window.GetTime() / 3, meshRotationAxes[i]);

      // Draw the meshes
      cubeMesh.Draw(window.camera, gBufferShader, m);
    }
    gBuffer.Unbind(GL_DRAW_FRAMEBUFFER);

    // Perform lighting pass
    window.MakeCurrent();
    window.Clear(GL_DEPTH_BUFFER_BIT);
    frameMesh.Draw(lightingShader);

    // Update the stuff
    window.Refresh();
  }

  return 0;
}
