#pragma once

#include "scene.hpp"

class MainScene : public Scene {
  public:
    void load(btDiscreteDynamicsWorld *world, Camera *camera, Model *groundModel, Model *skyboxModel, Model *cubeModel);
    void simulate(const btScalar delta);
    void reset();
  private:
    std::vector<Mesh> cubes;
    float explosionTimer;
};
