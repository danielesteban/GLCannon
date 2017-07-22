#pragma once

#include "model.hpp"

class CubeModel : public Model {
  public:
    CubeModel() : Model(VBO, VBOSIZE, EBO, EBOSIZE, EBOCOUNT, TEXTURE) {}
    void init(Shader *shader);
  private:
    static const GLfloat VBO[];
    static const GLsizei VBOSIZE;
    static const GLushort EBO[];
    static const GLsizei EBOSIZE;
    static const GLsizei EBOCOUNT;
    static const char TEXTURE[];
};
