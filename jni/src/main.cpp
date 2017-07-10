#include <imgui.h>
#include <imgui_impl_sdl_gles.h>
#include <vector>
#include "camera.hpp"
#include "fontawesome.hpp"
#include "mesh.hpp"
#include "meshes/button.hpp"
#include "models/button.hpp"
#include "models/cube.hpp"
#include "models/ground.hpp"
#include "models/skybox.hpp"
#include "shaders/button.hpp"
#include "shaders/skybox.hpp"
#include "shaders/standard.hpp"

#define FSAA 2

Camera camera;
ButtonModel fireButtonModel;
CubeModel cubeModel;
GroundModel groundModel;
SkyboxModel skyboxModel;
std::vector<Mesh> cubes;
Mesh ground;
Mesh skybox;
Button fireButton;

ButtonShader buttonShader;
StandardShader standardShader;
SkyboxShader skyboxShader;

btDefaultCollisionConfiguration *collisionConfiguration;
btCollisionDispatcher *dispatcher;
btBroadphaseInterface *overlappingPairCache;
btSequentialImpulseConstraintSolver *solver;
btDiscreteDynamicsWorld *world;

SDL_Window *gWindow = NULL;
SDL_GLContext gContext;

int width;
int height;

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
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.04f, 0.08f, 0.14f, 1.0f);
  SDL_GL_SetSwapInterval(1);
  buttonShader.init();
  standardShader.init();
  skyboxShader.init();
  resize();

  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  overlappingPairCache = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
  world->setGravity(btVector3(0.0f, -9.8f, 0.0f));

  ImGui_ImplSdlGLES_Init(gWindow);
  ImGuiIO& io = ImGui::GetIO();
  SDL_RWops *file = SDL_RWFromFile("roboto.ttf", "rb");
  int size = SDL_RWsize(file);
  Uint8 *roboto = new Uint8[size];
  SDL_RWread(file, roboto, size, 1);
  SDL_RWclose(file);
  io.Fonts->AddFontFromMemoryTTF(roboto, size, 32.0f);
}

void resize() {
  SDL_GL_GetDrawableSize(gWindow, &width, &height);
  glViewport(0, 0, width, height);
  camera.resize(width, height);
  glUseProgram(buttonShader.program);
  glUniformMatrix4fv(buttonShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection2D));
  glUseProgram(standardShader.program);
  glUniformMatrix4fv(standardShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
  glUseProgram(skyboxShader.program);
  glUniformMatrix4fv(skyboxShader.projection, 1, GL_FALSE, glm::value_ptr(camera.projection));
}

void spawnCube(btScalar force) {
  Mesh cube;
  const glm::vec3 origin = camera.position + camera.front;
  cube.init(world, &cubeModel, btVector3(origin[0], origin[1], origin[2]), btScalar(1.0f));
  cube.setRotation(btQuaternion(btVector3(
    ((float) rand() / (float) RAND_MAX) - 0.5f,
    ((float) rand() / (float) RAND_MAX) - 0.5f,
    ((float) rand() / (float) RAND_MAX) - 0.5f
  ), btScalar(glm::radians((float) (rand() % 360)))));
  cube.applyImpulse(btVector3(camera.front[0], camera.front[1], camera.front[2]) * force);
  cubes.push_back(cube);
}

int firingFinger = -1;
int motionFinger = -1;
void processTouch(const Uint32 event, const int finger, const float x, const float y, const float dx, const float dy) {
  const bool hoverFire = fireButton.isHover(x * camera.canvas2D[0], (1.0f - y) * camera.canvas2D[1]);
  if (
    fireButton.isFiring() && finger == firingFinger &&
    (
      event == SDL_FINGERUP ||
      (event == SDL_FINGERMOTION && !hoverFire)
    )
  ) {
    spawnCube(fireButton.getForce() * 32.0f);
    fireButton.setFiring(false);
    firingFinger = -1;
  }
  if (!fireButton.isFiring() && hoverFire && event == SDL_FINGERDOWN) {
    fireButton.setFiring(true);
    firingFinger = finger;
  }
  if (finger == firingFinger) return;

  if (event == SDL_FINGERDOWN && motionFinger == -1) {
    motionFinger = finger;
  }
  if (event == SDL_FINGERMOTION && finger == motionFinger) {
    camera.processInput(dx, dy);
  }
  if (event == SDL_FINGERUP && finger == motionFinger) {
    motionFinger = -1;
  }
}

void setupScene() {
  srand(time(NULL));
  cubeModel.init(&standardShader);
  groundModel.init(&standardShader);
  fireButtonModel.init(&buttonShader, "fire");
  ground.init(world, &groundModel, btVector3(0.0f, -1.0f, 0.0f));
  skyboxModel.init(&skyboxShader);
  skybox.init(NULL, &skyboxModel, btVector3(camera.position[0], camera.position[1], camera.position[2]));
  fireButton.init(&fireButtonModel, btVector3(camera.canvas2D[0] - 96.0f, 96.0f, 0.0f));
}

void simulateScene(const btScalar delta) {
  world->stepSimulation(delta, 4);
  for (std::vector<Mesh>::iterator cube = cubes.begin(); cube != cubes.end(); ++cube) {
    (*cube).simulate(delta);
  }
  fireButton.simulate(delta);
}

void renderScene() {
  glUseProgram(standardShader.program);
  ground.render(&camera);
  for (std::vector<Mesh>::iterator cube = cubes.begin(); cube != cubes.end(); ++cube) {
    (*cube).render(&camera);
  }
  glUseProgram(skyboxShader.program);
  glDepthFunc(GL_LEQUAL);
  skybox.render(&camera);
  glDepthFunc(GL_LESS);
}

void renderUI() {
  glUseProgram(buttonShader.program);
  glEnable(GL_BLEND);
  fireButton.render();
  glDisable(GL_BLEND);

  ImGui_ImplSdlGLES_NewFrame(gWindow);
  bool open = true;
  ImGui::Begin("DEBUG", &open, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
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
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          return;
        case SDL_WINDOWEVENT_RESIZED:
          resize();
          break;
        case SDL_FINGERDOWN:
        case SDL_FINGERMOTION:
        case SDL_FINGERUP:
          processTouch(e.type, e.tfinger.fingerId, e.tfinger.x, e.tfinger.y, e.tfinger.dx, e.tfinger.dy);
          break;
      }
    }
    unsigned int currentTicks = SDL_GetTicks();
    const btScalar delta = btScalar(currentTicks - lastTicks) / btScalar(1000.0f);
    lastTicks = currentTicks;
    simulateScene(delta);
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
