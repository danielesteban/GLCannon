#ifndef MESH_H
#define MESH_H

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "camera.hpp"
#include "model.hpp"

class Mesh {
  public:
    virtual void init(Model *model, const glm::vec3 position);
    virtual void render(const Camera *camera);
    void setPosition(const glm::vec3 position);
    void setRotation(const glm::quat rotation);
    virtual void animate();
  private:
    void updateTransform();
    Model *model;
    glm::vec3 position;
    glm::quat rotation;
    glm::mat4 transform;
};

#endif
