#ifndef MESH_H
#define MESH_H

#include <SDL.h>
#include <SDL_opengles2.h>
#include "camera.hpp"
#include "shader.hpp"

class Mesh {
  public:
    void render(Camera *camera, Shader *shader);
  private:
    static const float vertices[];
    static const float colors[];
    glm::mat4 transform;
};

#endif
