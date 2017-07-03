#ifndef STANDARD_SHADER_H
#define STANDARD_SHADER_H

#include "shader.hpp"

class StandardShader : public Shader {
  public:
    StandardShader() : Shader(VERTEX, FRAGMENT) {}
  private:
    static const char *VERTEX;
    static const char *FRAGMENT;
};

#endif
