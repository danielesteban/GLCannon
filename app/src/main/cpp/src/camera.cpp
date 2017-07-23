#include "camera.hpp"

const float Camera::fov = glm::radians(60.0f);
const float Camera::maxTilt = glm::radians(360.0f);
const float Camera::maxPitch = glm::radians(85.0f);
const float Camera::sensitivity = 3.0f;
const glm::vec3 Camera::worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera() {
  position = glm::vec3(0.0f, 1.62f, 0.0f);
  tilt = glm::radians(90.0f);
  pitch = glm::radians(8.0f);
}

void Camera::processInput(float dx, float dy) {
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
  projection = glm::perspective(fov, aspect, 0.1f, 512.0f);
  canvas2D = glm::vec2(1024.0f, (float) h * 1024.0f / (float) w);
  projection2D = glm::ortho(0.0f, canvas2D[0], 0.0f, canvas2D[1]);
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
