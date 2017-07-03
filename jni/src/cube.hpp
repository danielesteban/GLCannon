#ifndef CUBE_H
#define CUBE_H

#include "mesh.hpp"

class Cube : public Mesh {
  public:
    Cube() : Mesh(VBO, EBO, SIZE, COUNT, TEXTURE) {}
  private:
    static const GLfloat VBO[];
    static const GLushort EBO[];
    static const GLsizei SIZE;
    static const GLsizei COUNT;
    static const char TEXTURE[];
};

#endif
