#pragma once

#include <SDL.h>
#include <SDL_image.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <btBulletDynamicsCommon.h>
#include <glm.hpp>
#include <gtc/matrix_inverse.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "camera.hpp"
#include "model.hpp"

class Mesh {
  public:
    virtual void init(
      btDiscreteDynamicsWorld *world,
      Model *model,
      const btVector3 position = btVector3(),
      const btQuaternion rotation = btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
      const btScalar mass = btScalar(0.0f)
    );
    virtual void destroy();
    virtual void render(const Camera *camera);
    virtual void simulate(const btScalar delta);
    void reset();
    void setPosition(const btVector3 position);
    void setRotation(const btQuaternion rotation);
    void applyImpulse(const btVector3 impulse);
  protected:
    Model *model;
    btTransform transform;
    void updateView();
  private:
    btTransform initialTransform;
    btRigidBody *body;
    btDiscreteDynamicsWorld *world;
    glm::mat4 view;
    glm::mat3 normalView;
};
