#pragma once

#include "scene.hpp"

class TestScene : public Scene {
  public:
    void load(btDiscreteDynamicsWorld *world, Camera *camera, Model *groundModel, Model *skyboxModel, Model *cubeModel);
};
