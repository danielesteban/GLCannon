#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include "shader.hpp"

class SkyboxShader : public Shader {
  public:
    SkyboxShader() : Shader(VERTEX, FRAGMENT) {}
  private:
    static const char *VERTEX;
    static const char *FRAGMENT;
};

#endif
