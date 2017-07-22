#pragma once

#include "mesh.hpp"

class Button : public Mesh {
  public:
    void init(Model *model, const btVector3 position);
    void render(const Camera *camera = NULL);
    void simulate(const btScalar delta);
    bool isHover(const float x, const float y);
    bool isFiring();
    void setFiring(const bool state);
    btScalar getForce();
  private:
    bool firing;
    bool raising;
    btScalar force;
    glm::vec4 albedo;
};
