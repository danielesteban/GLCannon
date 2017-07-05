#include "camera.hpp"
#include "mesh.hpp"
#include "shaders/skybox.hpp"
#include "shaders/standard.hpp"
#include "models/cube.hpp"
#include "models/ground.hpp"
#include "models/skybox.hpp"
#include <imgui.h>
#include <imgui_impl_sdl_gles.h>

#define FSAA 1
#define CUBE_COUNT 512
#define CUBE_RADIUS 128

Camera camera;
Cube cubeModel;
Ground groundModel;
Skybox skyboxModel;
Mesh cubes[CUBE_COUNT];
Mesh ground;
Mesh skybox;

StandardShader standardShader;
SkyboxShader skyboxShader;

btDefaultCollisionConfiguration *collisionConfiguration;
btCollisionDispatcher *dispatcher;
btBroadphaseInterface *overlappingPairCache;
btSequentialImpulseConstraintSolver *solver;
btDiscreteDynamicsWorld *world;

SDL_Window *gWindow = NULL;
SDL_GLContext gContext;

void resize();
void init() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, FSAA);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  gWindow = SDL_CreateWindow("GLTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_OPENGL);
  gContext = SDL_GL_CreateContext(gWindow);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.04f, 0.08f, 0.14f, 1.0f);
  SDL_GL_SetSwapInterval(1);
  standardShader.init();
  skyboxShader.init();
  resize();
  ImGui_ImplSdlGLES_Init(gWindow);

  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  overlappingPairCache = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
  world->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

void resize() {
  int w, h;
  SDL_GL_GetDrawableSize(gWindow, &w, &h);
  glViewport(0, 0, w, h);
  camera.resize(w, h);
  glUseProgram(standardShader.program);
  glUniformMatrix4fv(standardShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
  glUseProgram(skyboxShader.program);
  glUniformMatrix4fv(skyboxShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
}

void processTouch(int finger, float dx, float dy) {
  if (finger > 0) return;
  camera.processInput(dx, dy);
}

void setupScene() {
  srand(time(NULL));
  cubeModel.init(&standardShader);
  for (int i = 0; i < CUBE_COUNT; i += 1) {
    cubes[i].init(world, &cubeModel, btVector3(
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f
    ) * btVector3(
      (float) (rand() % CUBE_RADIUS),
      (float) (rand() % (CUBE_RADIUS * 2)) + (float) CUBE_RADIUS,
      (float) (rand() % CUBE_RADIUS)
    ), btScalar(0.5f));
    cubes[i].setRotation(btQuaternion(btVector3(
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f,
      ((float) rand() / (float) RAND_MAX) - 0.5f
    ), btScalar(glm::radians((float) (rand() % 360)))));
  }
  groundModel.init(&standardShader);
  ground.init(world, &groundModel, btVector3(0.0f, -1.0f, 0.0f));
  skyboxModel.init(&skyboxShader);
  skybox.init(world, &skyboxModel, btVector3(camera.position[0], camera.position[1], camera.position[2]));
}

void animateScene(const btScalar delta) {
  world->stepSimulation(delta, 4);
  for (int i = 0; i < CUBE_COUNT; i += 1) {
    cubes[i].animate(delta);
  }
}

void renderScene() {
  glUseProgram(standardShader.program);
  for (int i = 0; i < CUBE_COUNT; i += 1) {
    cubes[i].render(&camera);
  }
  ground.render(&camera);
  glUseProgram(skyboxShader.program);
  glDepthFunc(GL_LEQUAL);
  skybox.render(&camera);
  glDepthFunc(GL_LESS);
}

void renderUI() {
  ImGui_ImplSdlGLES_NewFrame(gWindow);
  bool open = true;
  ImGui::Begin("UI", &open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
  ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
  ImGui::Text("%dx%d FSAA x%d", (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y, FSAA);
  ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::GetWindowWidth() - 9, 8));
  ImGui::End();
  ImGui::Render();
}

void loop() {
  SDL_Event e;
  bool quit = false;
  unsigned int lastTicks = SDL_GetTicks();
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      ImGui_ImplSdlGLES_ProcessEvent(&e);
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          return;
        case SDL_WINDOWEVENT_RESIZED:
          resize();
          break;
        case SDL_FINGERDOWN:
        case SDL_FINGERMOTION:
          processTouch(e.tfinger.fingerId, e.tfinger.dx, e.tfinger.dy);
          break;
      }
    }
    unsigned int currentTicks = SDL_GetTicks();
    const btScalar delta = btScalar(currentTicks - lastTicks) / btScalar(1000.0f);
    lastTicks = currentTicks;
    animateScene(delta);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene();
    renderUI();
    SDL_GL_SwapWindow(gWindow);
  }
}

int main(int argc, char *args[]) {
  init();
  setupScene();
  loop();
  return 0;
}
