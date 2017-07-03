#ifndef ROTATING_CUBE_MESH_H
#define ROTATING_CUBE_MESH_H

#include "mesh.hpp"

class RotatingCube : public Mesh {
  public:
    void init(Model *model, const int radius);
    void animate();
  private:
    glm::vec3 rotationAxis;
    float rotationSpeed;
};

#endif
