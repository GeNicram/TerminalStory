#ifndef ENTITY_H
#define ENTITY_H

#include <ncurses.h>
#include <vector>

class Entity {
public:
  Entity();
  virtual ~Entity() {}
  void Draw(WINDOW* window);
  void DrawTown(WINDOW* window);
  void Do(float _d);
  void DoHero(int g, float _d);
  void DoHeroAttack();
  void DoTownAttack();
  float CountPower() {
    return (1+t_power->level)*
           (level + attack_power);
  }
  void DoTown(float _d);

  float x, y;
  char* model;
  int hp;
  int level;
  float speed;
  float exp;

  char type;

  // Only for Hero
  float attack_life;
  float attack_size;
  float attack_power;
  int money;
  char face_dir;

  // Only for Town
  int cost;

  static std::vector<Entity>* enemy;
  static Entity* hero;
  static Entity* t_center;
  static Entity* t_house;
  static Entity* t_power;
  static Entity* t_army;
};

#endif
