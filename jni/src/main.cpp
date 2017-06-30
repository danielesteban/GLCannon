#include <SDL.h>
#include <SDL_opengles2.h>

#include "camera.hpp"
#include "mesh.hpp"
#include "shader.hpp"

SDL_Window* gWindow = NULL;
SDL_GLContext gContext;

Camera camera;
Mesh mesh;
Shader shader;

void resize();
void init() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  gWindow = SDL_CreateWindow("GLTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL);
  gContext = SDL_GL_CreateContext(gWindow);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  SDL_GL_SetSwapInterval(1);
  shader.init();
  resize();
}

void resize() {
  int w, h;
  SDL_GL_GetDrawableSize(gWindow, &w, &h);
  glViewport(0, 0, w, h);
  camera.resize(w, h);
}

void processTouch(int finger, float x, float y) {
  if (finger > 0) return;
  glClearColor(x, 0.0f, y, 1.0f);
  camera.position[0] = (x - 0.5f) * 6.0f;
  camera.position[2] = 4.5f + (y - 0.5f) * 3.0f;
  camera.updateTransform();
  SDL_Log("%d %d", (int) (x * 100.0f), (int) (y * 100.0f));
}

void loop() {
  SDL_bool quit = SDL_FALSE;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT:
          quit = SDL_TRUE;
          break;
        case SDL_WINDOWEVENT_RESIZED:
          resize();
          break;
        case SDL_FINGERDOWN:
        case SDL_FINGERMOTION:
          processTouch(e.tfinger.fingerId, e.tfinger.x, e.tfinger.y);
          break;
      }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh.render(&camera, &shader);
    SDL_GL_SwapWindow(gWindow);
  }
}

int main(int argc, char* args[]) {
  init();
  loop();
  return 0;
}
