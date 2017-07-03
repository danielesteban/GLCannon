#ifndef MESH_H
#define MESH_H

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include "camera.hpp"
#include "model.hpp"

class Mesh {
  public:
    void init(Model *model, const glm::vec3 position);
    void render(Camera *camera);
    void setPosition(const glm::vec3 position);
    void setRotation(const glm::quat rotation);
    void updateTransform();
  private:
    Model *model;
    glm::vec3 position;
    glm::quat rotation;
    glm::mat4 transform;
};

#endif
