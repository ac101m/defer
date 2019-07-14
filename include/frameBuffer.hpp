#ifndef DEFER_FRAMEBUFFER_INCLUDED
#define DEFER_FRAMEBUFFER_INCLUDED


// Standard
#include <vector>

// External
#include <GLT/Texture.hpp>


// Frame buffer class
class FrameBuffer : GLT::RefCount {
private:
  std::vector<GLT::Texture> textures;
  GLuint glHandle;

public:

  // Constructor
  FrameBuffer(int width, int height) {

    // Create frameBuffer object
    glGenFramebuffers(1, &this->glHandle);
    this->Bind();

    // Not a good idea
    unsigned int gPosition, gNormal, gAlbedoSpec;

    // position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->textures.push_back(GLT::Texture(gPosition));

    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->textures.push_back(GLT::Texture(gNormal));

    // color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->textures.push_back(GLT::Texture(gAlbedoSpec));

    // Attach textures to frame buffer
    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D,
      gPosition, 0);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT1,
      GL_TEXTURE_2D,
      gNormal, 0);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT2,
      GL_TEXTURE_2D,
      gAlbedoSpec, 0);

    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

/*
    // Generate textures with specified formats
    std::vector<GLuint> attachments;
    for(unsigned i = 0; i < this->textures.size(); i++) {

      // Add the texture to the framebuffer
      this->textures[i].Bind();
      glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0 + i,
        GL_TEXTURE_2D,
        this->textures[i].GetGlHandle(), 0);

      // Add texture attachment to attachment list
      attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    // Indicate to opengl which attachments to use
    glDrawBuffers(attachments.size(), attachments.data());
*/
    // Add the depth buffer as an rbo
    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(
      GL_RENDERBUFFER,
      GL_DEPTH_COMPONENT,
      width, height);
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT,
      GL_RENDERBUFFER,
      rboDepth);

    // Check that the frame buffer is valid
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << "Error, failed to construct frame buffer, errorcode: ";
      std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
      exit(1);
    }

    // Unbind or frame buffer
    this->Unbind();
  }

  // Bind and unbind the framebuffer for drawing
  void Bind(void) {glBindFramebuffer(GL_FRAMEBUFFER, this->glHandle);}
  void Unbind(void) {glBindFramebuffer(GL_FRAMEBUFFER, 0);}

  // Get internal textures
  std::vector<GLT::Texture> GetTextures(void) {
    return this->textures;
  }

  // Clear to black
  void Clear(void) {
    this->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  // Destructor
  ~FrameBuffer(void) {
    if(!this->ReferencedElsewhere()) {
      glDeleteFramebuffers(1, &this->glHandle);
    }
  }
};


#endif // DEFER_FRAMEBUFFER_INCLUDED
