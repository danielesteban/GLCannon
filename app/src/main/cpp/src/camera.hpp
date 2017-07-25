#pragma once

#include <SDL.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera {
  public:
    Camera();
    void processInput(float dx, float dy);
    void resize(int w, int h);
    void updateVectors();
    void updateView();
    float tilt;
    float pitch;
    glm::vec3 position;
    glm::mat4 projection;
    glm::mat4 projection2D;
    glm::vec2 canvas2D;
    glm::mat4 view;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
  private:
    static const float fov;
    static const float maxTilt;
    static const float maxPitch;
    static const float sensitivity;
    static const glm::vec3 worldUp;

};
