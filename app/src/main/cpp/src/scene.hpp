#pragma once

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "camera.hpp"
#include "mesh.hpp"

class Scene {
  public:
    virtual void load(btDiscreteDynamicsWorld *world, Camera *camera, Model *groundModel, Model *skyboxModel, Model *cubeModel) = 0;
    virtual void unload();
    virtual void simulate(const btScalar delta);
    virtual void render(const Camera *camera);
    virtual void reset();
    const bool isResetting();
    const GLfloat* getFogColor();
    const GLfloat* getSunPosition();
  protected:
    std::vector<Mesh> meshes;
    bool resetting;
    glm::vec4 fogColor;
    glm::vec3 sunPosition;
};
