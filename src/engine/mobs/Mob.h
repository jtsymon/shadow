#ifndef MOB_H
#define	MOB_H

#include "../Entity.h"

class Mob : public Entity {
 public:

 Mob(Vector<int> pos, Map *map) : Entity::Entity(pos, map) {
  }
  void tick(Vector<int> target);

};

#endif	/* MOB_H */
