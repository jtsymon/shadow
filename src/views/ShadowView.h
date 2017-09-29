#pragma once

#include <pthread.h>
#include <semaphore.h>

#include "GameView.h"
#include "../map/Map.h"
#include "../engine/Entity.h"
#include "../engine/mobs/Mob.h"

class ShadowView : public GameView {
  pthread_t* execution;
  pthread_t* shading;
  Map map;
  Entity player;
  std::vector<Mob> mobs;
 public:
  ShadowView();
  ~ShadowView();
  void show();
  void hide();
  void render();
  void key_callback(int key, int scancode, int action, int mods);
  void mouse_callback(int button, int action, int mods);
  void window_size_callback(int width, int height);
};
