#include "button.hpp"

void Button::init(Model *model, const btVector3 position) {
  Mesh::init(NULL, model, position);
  albedo = glm::vec4(0.1f, 0.1f, 0.1f, 0.9f);
}

bool Button::isHover(const float x, const float y) {
  const btVector3 position = transform.getOrigin();
  if (x < position.getX() - 128.0f) return false;
  if (x > position.getX() + 128.0f) return false;
  if (y < position.getY() - 64.0f) return false;
  if (y > position.getY() + 64.0f) return false;
  return true;
}

void Button::render(const Camera *camera) {
  glUniform4fv(model->shader->albedo, 1, glm::value_ptr(albedo));
  Mesh::render(NULL);
}

void Button::animate(const btScalar delta) {
  const btScalar step = 2.0f * delta;
  if (!firing) return;
  if (raising) {
    force += step;
    if (force > 1.0f) {
      force = 1.0f;
      raising = false;
    }
  } else {
    force -= step;
    if (force < 0.0f) {
      force = 0.0f;
      raising = true;
    }
  }
  albedo[0] = fmax(0.1f, force);
}

bool Button::isFiring() {
  return firing;
}

void Button::setFiring(const bool state) {
  firing = state;
  raising = true;
  force = 0.0f;
  albedo[0] = 0.1f;
}

btScalar Button::getForce() {
  return force;
}
