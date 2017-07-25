#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>
#include <imgui.h>
#include <imgui_impl_sdl_gles.h>
#include <jni.h>
#include <vector>
#include "camera.hpp"
#include "mesh.hpp"
#include "meshes/button.hpp"
#include "meshes/menu.hpp"
#include "meshes/skybox.hpp"
#include "meshes/sphere.hpp"
#include "models/button.hpp"
#include "models/cube.hpp"
#include "models/ground.hpp"
#include "models/menu.hpp"
#include "models/skybox.hpp"
#include "models/sphere.hpp"
#include "scene.hpp"
#include "scenes/main.hpp"
#include "scenes/test.hpp"
#include "shader.hpp"
#include "shaders/button.hpp"
#include "shaders/menu.hpp"
#include "shaders/skybox.hpp"
#include "shaders/sphere.hpp"
#include "shaders/standard.hpp"

#define FSAA 2
#define MAX_SPHERES 16

SDL_Window *gWindow = NULL;
SDL_GLContext gContext;

int width = 0;
int height = 0;

Camera camera;

ButtonShader buttonShader;
MenuShader menuShader;
StandardShader standardShader;
SkyboxShader skyboxShader;
SphereShader sphereShader;

Shader *shaders[] = {
  &buttonShader,
  &menuShader,
  &skyboxShader,
  &sphereShader,
  &standardShader
};

CubeModel cubeModel;
GroundModel groundModel;
ButtonModel fireButtonModel;
MenuModel menuModel;
SkyboxModel skyboxModel;
SphereModel sphereModel;

Button fireButton;
Menu menu;
Skybox skybox;
std::vector<Sphere> spheres;

Scene *scene = NULL;
int sceneIndex = -1;
enum {
  SCENE_MAIN,
  SCENE_TEST,
  NUM_SCENES,
};
MainScene mainScene;
TestScene testScene;
Scene *scenes[] = {
  &mainScene,
  &testScene,
};

btDefaultCollisionConfiguration *collisionConfiguration;
btCollisionDispatcher *dispatcher;
btBroadphaseInterface *overlappingPairCache;
btSequentialImpulseConstraintSolver *solver;
btDiscreteDynamicsWorld *world;

Mix_Chunk *cannonSound = NULL;

void resize();
void init() {
  SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, FSAA);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  gWindow = SDL_CreateWindow("GLCannon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
  gContext = SDL_GL_CreateContext(gWindow);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  SDL_GL_SetSwapInterval(1);
  for (int i = 0; i < sizeof(shaders) / sizeof(Shader*); i += 1) {
    shaders[i]->init();
  }
  resize();

  srand(time(NULL));

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

  cannonSound = Mix_LoadWAV("cannon.ogg");

  fireButtonModel.init(&buttonShader, "fire");
  fireButton.init(&fireButtonModel, btVector3(camera.canvas2D[0] - 96.0f, 96.0f, 0.0f));
  menuModel.init(&menuShader, "menu", 6);
  menu.init(&menuModel, camera.canvas2D[1] - MenuModel::itemSize);

  cubeModel.init(&standardShader);
  groundModel.init(&standardShader);
  skyboxModel.init(&skyboxShader);
  sphereModel.init(&sphereShader);
  skybox.init(&skyboxModel);
}

void resize() {
  SDL_GL_GetDrawableSize(gWindow, &width, &height);
  glViewport(0, 0, width, height);
  camera.resize(width, height);
  for (int i = 0; i < sizeof(shaders) / sizeof(Shader*); i += 1) {
    glUseProgram(shaders[i]->program);
    GLfloat *projection;
    if (shaders[i] == &buttonShader || shaders[i] == &menuShader) {
      projection = glm::value_ptr(camera.projection2D);
    } else {
      projection = glm::value_ptr(camera.projection);
    }
    glUniformMatrix4fv(shaders[i]->projection, 1, GL_FALSE, projection);
  }
}

void openGitHub() {
  JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv();
  jobject activity = (jobject)SDL_AndroidGetActivity();
  jclass clazz(env->GetObjectClass(activity));
  jmethodID method_id = env->GetMethodID(clazz, "openGitHub", "()V");
  env->CallVoidMethod(activity, method_id);
  env->DeleteLocalRef(activity);
  env->DeleteLocalRef(clazz);
}

void playSound(Mix_Chunk *sound, const float volume) {
  int channel = Mix_PlayChannel(-1, sound, 0);
  if (channel != -1) Mix_Volume(channel, (int) ((float) MIX_MAX_VOLUME * volume));
}

void spawnSphere(btScalar force) {
  if (spheres.size() >= MAX_SPHERES) {
    spheres.front().destroy();
    spheres.erase(spheres.begin());
  }
  const btScalar power = btScalar(menu.getActiveItem() + 2);
  Sphere sphere;
  sphere.init(world, &sphereModel, btVector3(camera.position[0], camera.position[1], camera.position[2]));
  sphere.applyImpulse(btVector3(camera.front[0], camera.front[1], camera.front[2]) * 50.0f * power * force);
  spheres.push_back(sphere);
  playSound(cannonSound, 0.2f * power * force);
}

void clearSpheres() {
  if (!spheres.empty()) {
    for (std::vector<Sphere>::iterator sphere = spheres.begin(); sphere != spheres.end(); ++sphere) {
      (*sphere).destroy();
    }
    spheres.clear();
  }
}

void loadScene(const int index) {
  if (scene != NULL) {
    scene->unload();
    clearSpheres();
  }
  scene = scenes[index];
  sceneIndex = index;
  scene->load(world, &camera, &groundModel, &skyboxModel, &cubeModel);
  for (int i = 0; i < sizeof(shaders) / sizeof(Shader*); i += 1) {
    glUseProgram(shaders[i]->program);
    if(shaders[i]->fogColor != -1) {
      glUniform4fv(shaders[i]->fogColor, 1, scene->getFogColor());
    }
    if(shaders[i]->sunPosition != -1) {
      glUniform3fv(shaders[i]->sunPosition, 1, scene->getSunPosition());
    }
  }
}

int firingFinger = -1;
int menuFinger = -1;
int motionFinger = -1;
void processTouch(const Uint32 event, const int finger, const float x, const float y, const float dx, const float dy) {
  const float UIx = x * camera.canvas2D[0];
  const float UIy = (1.0f - y) * camera.canvas2D[1];
  const bool hoverFire = fireButton.isHover(UIx, UIy);
  if (
    fireButton.isFiring() && finger == firingFinger &&
    (
      event == SDL_FINGERUP ||
      (event == SDL_FINGERMOTION && !hoverFire)
    )
  ) {
    spawnSphere(fireButton.getForce());
    fireButton.setFiring(false);
    firingFinger = -1;
  }
  if (!scene->isResetting() && !fireButton.isFiring() && hoverFire && event == SDL_FINGERDOWN) {
    fireButton.setFiring(true);
    firingFinger = finger;
  }
  if (finger == firingFinger) return;

  const bool hoverMenu = menu.isHover(UIx, UIy);
  if (hoverMenu && event == SDL_FINGERDOWN) {
    menuFinger = finger;
  }
  if (finger == menuFinger) {
    if (hoverMenu) {
      if (event == SDL_FINGERUP) {
        const int item = menu.click(UIx);
        switch (item) {
          case 0:
            loadScene((sceneIndex + 1) % NUM_SCENES);
          break;
          case 3:
            if (!scene->isResetting()) {
              clearSpheres();
              scene->reset();
              playSound(cannonSound, 1.0f);
            }
            break;
          case 4:
            openGitHub();
            break;
          case 5:
            menu.toggle();
            break;
          default:
            menu.setActiveItem(item);
        }
      }
      return;
    }
    menuFinger = -1;
  }

  if ((event == SDL_FINGERDOWN || event == SDL_FINGERMOTION) && motionFinger == -1) {
    motionFinger = finger;
  }
  if (event == SDL_FINGERMOTION && finger == motionFinger) {
    camera.processInput(dx, dy);
  }
  if (event == SDL_FINGERUP && finger == motionFinger) {
    motionFinger = -1;
  }
}

void simulate(const btScalar delta) {
  world->stepSimulation(delta, 4);
  fireButton.simulate(delta);
  menu.simulate(delta);
  scene->simulate(delta);
  for (std::vector<Sphere>::iterator sphere = spheres.begin(); sphere != spheres.end(); ++sphere) {
    (*sphere).simulate(delta);
  }
}

void render() {
  /* Scene */
  scene->render(&camera);
  glUseProgram(sphereShader.program);
  for (std::vector<Sphere>::iterator sphere = spheres.begin(); sphere != spheres.end(); ++sphere) {
    (*sphere).render(&camera);
  }
  glUseProgram(skyboxShader.program);
  glDepthFunc(GL_LEQUAL);
  skybox.render(&camera);
  glDepthFunc(GL_LESS);

  /* UI */
  glUseProgram(buttonShader.program);
  glEnable(GL_BLEND);
  fireButton.render();
  glUseProgram(menuShader.program);
  menu.render();
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
    simulate(delta);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();
    SDL_GL_SwapWindow(gWindow);
  }
}

int main(int argc, char *args[]) {
  init();
  loadScene(SCENE_MAIN);
  loop();
  return 0;
}
