#pragma once

#include "model.hpp"

class ButtonModel : public Model {
  public:
    ButtonModel() : Model(VBO, VBOSIZE, EBO, EBOSIZE, EBOCOUNT) {}
    void init(Shader *shader);
  private:
    static const GLfloat VBO[];
    static const GLsizei VBOSIZE;
    static const GLushort EBO[];
    static const GLsizei EBOSIZE;
    static const GLsizei EBOCOUNT;
};
