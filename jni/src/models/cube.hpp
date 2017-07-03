#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include "model.hpp"

class Cube : public Model {
  public:
    Cube() : Model(VBO, VBOSIZE, EBO, EBOSIZE, EBOCOUNT, TEXTURE) {}
  private:
    static const GLfloat VBO[];
    static const GLsizei VBOSIZE;
    static const GLushort EBO[];
    static const GLsizei EBOSIZE;
    static const GLsizei EBOCOUNT;
    static const char TEXTURE[];
};

#endif
