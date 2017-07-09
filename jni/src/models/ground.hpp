#pragma once

#include "model.hpp"

class Ground : public Model {
  public:
    Ground() : Model(VBO, VBOSIZE, EBO, EBOSIZE, EBOCOUNT, TEXTURE) {}
    void init(Shader *shader);
  private:
    static const GLfloat VBO[];
    static const GLsizei VBOSIZE;
    static const GLushort EBO[];
    static const GLsizei EBOSIZE;
    static const GLsizei EBOCOUNT;
    static const char TEXTURE[];
};
