#pragma once

#include "shader.hpp"

class SkyboxShader : public Shader {
  public:
    SkyboxShader() : Shader(VERTEX, FRAGMENT) {}
  private:
    static const char *VERTEX;
    static const char *FRAGMENT;
};
