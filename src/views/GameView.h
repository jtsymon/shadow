#ifndef SCREEN_GAME_H
#define SCREEN_GAME_H

#include <pthread.h>
#include <semaphore.h>

#include "../View.h"
#include "../map/Map.h"
#include "../engine/Entity.h"
#include "../engine/mobs/Mob.h"

class GameView : public View {
  pthread_t* execution;
  pthread_t* shading;
  Map *map;
  Entity player;
  std::vector<Mob> mobs;
 public:
  GameView();
  ~GameView();
  void show();
  void hide();
  void render();
  void key_callback(int key, int scancode, int action, int mods);
  void mouse_callback(int button, int action, int mods);
};


#endif /* SCREEN_GAME_H */
