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
  model->render(glm::value_ptr(camera ? (camera->view * view) : view), glm::value_ptr(normalView));
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

void Mesh::applyImpulse(const btVector3 impulse) {
  body->activate(true);
  body->applyCentralImpulse(impulse);
}

void Mesh::updateView() {
  transform.getOpenGLMatrix(glm::value_ptr(view));
  normalView = glm::inverseTranspose(glm::mat3(view));
}

void Mesh::simulate(const btScalar delta) {
  if (body != NULL && body->isActive()) {
    body->getMotionState()->getWorldTransform(transform);
    updateView();
  }
}
