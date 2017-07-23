#pragma once

#include "shader.hpp"

class SphereShader : public Shader {
  public:
    SphereShader() : Shader(VERTEX, FRAGMENT) {}
  private:
    static const char *VERTEX;
    static const char *FRAGMENT;
};
