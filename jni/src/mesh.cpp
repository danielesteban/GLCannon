#include "mesh.hpp"

void Mesh::init(btDiscreteDynamicsWorld *world, Model *model, const btVector3 position, const btScalar mass) {
  this->model = model;
  transform.setIdentity();
  transform.setOrigin(btVector3(position[0], position[1], position[2]));
  if (model->collision != NULL) {
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic) {
      model->collision->calculateLocalInertia(mass, localInertia);
    }
    btDefaultMotionState *motionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, model->collision, localInertia);
		body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
  } else {
    body = NULL;
  }
  updateView();
}

void Mesh::render(const Camera *camera) {
  model->render(glm::value_ptr(camera->view * view));
}

void Mesh::setPosition(const btVector3 position) {
  transform.setOrigin(position);
  body->setWorldTransform(transform);
  updateView();
}

void Mesh::setRotation(const btQuaternion rotation) {
  transform.setRotation(rotation);
  body->setWorldTransform(transform);
  updateView();
}

void Mesh::updateView() {
  transform.getOpenGLMatrix(glm::value_ptr(view));
}

void Mesh::animate(const btScalar delta) {
  if (body != NULL) {
    body->getMotionState()->getWorldTransform(transform);
    updateView();
  }
}
