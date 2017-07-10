#pragma once

#include "model.hpp"

class ButtonModel : public Model {
  public:
    ButtonModel() : Model(VBO, VBOSIZE, EBO, EBOSIZE, EBOCOUNT, NULL, GL_TEXTURE_2D, GL_RGBA, GL_CLAMP_TO_EDGE) {}
    void init(Shader *shader, const char *texture);
  private:
    static const GLfloat VBO[];
    static const GLsizei VBOSIZE;
    static const GLushort EBO[];
    static const GLsizei EBOSIZE;
    static const GLsizei EBOCOUNT;
};
