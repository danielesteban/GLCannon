#include "camera.hpp"

const glm::vec3 Camera::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() {
  tilt = glm::radians(270.0f);
  pitch = 0;
  position[0] = 0.0f;
  position[1] = 1.62f;
  position[2] = 0.0f;
}

void Camera::processInput(float dx, float dy) {
  const float sensitivity = 3.0f;
  const float maxTilt = glm::radians(360.0f);
  const float maxPitch = glm::radians(85.0f);

  tilt += dx * sensitivity;
  if (tilt < 0) tilt += maxTilt;
  if (tilt > maxTilt) tilt -= maxTilt;

  pitch += dy * -sensitivity;
  if (pitch > maxPitch) pitch = maxPitch;
  if (pitch < -maxPitch) pitch = -maxPitch;

  updateView();
}

void Camera::resize(int w, int h) {
  const float aspect = (float) w / (float) h;
  projection = glm::perspective(glm::radians(75.0f), aspect, 0.1f, 512.0f);
  updateView();
}

void Camera::updateView() {
  front[0] = cos(tilt) * cos(pitch);
  front[1] = sin(pitch);
  front[2] = sin(tilt) * cos(pitch);
  front = glm::normalize(front);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
  view = glm::lookAt(
    position,
    position + front,
    up
  );
}
