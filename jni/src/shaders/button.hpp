#pragma once

#include "shader.hpp"

class ButtonShader : public Shader {
  public:
    ButtonShader() : Shader(VERTEX, FRAGMENT) {}
  private:
    static const char *VERTEX;
    static const char *FRAGMENT;
};
