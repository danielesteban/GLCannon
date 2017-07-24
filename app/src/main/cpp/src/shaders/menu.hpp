#pragma once

#include "shader.hpp"

class MenuShader : public Shader {
  public:
    MenuShader() : Shader(VERTEX, FRAGMENT) {}
  private:
    static const char *VERTEX;
    static const char *FRAGMENT;
};
