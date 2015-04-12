#ifndef RENDER_H
#define RENDER_H

#include "entity.h"

#include <ncurses.h>
#include <vector>

class Render {
public:
  Render();
  ~Render();


  void Draw(float _d);
  void DrawPanel(float _d);
  void Clear();
  void ClearGame();
  void ClearPanel();
  void SayHello();
  void SayVersion(int _x, int _y);

  void SetHero(Entity* _hero) {
    hero_ = _hero;
  }

  void SetEnemy(std::vector<Entity>* _enemy) {
    enemy_ = _enemy;
  }

  void SetTown(Entity* _center, Entity* _house,
               Entity* _power, Entity* _army) {
    t_center_ = _center;
    t_house_ = _house;
    t_power_ = _power;
    t_army_ = _army;
  }
  
  void Winner();
  void Looser();
  void Title();

  enum colors {Grass = 1, Hero, Panel, Enemy, Message};

  static const int screen_w;
  static const int screen_h;

  static int button_;
private:
  WINDOW* CreateWindow(int height, int width, int starty, int startx);
  WINDOW* wgame_;
  WINDOW* wbar_;

  Entity *hero_;
  std::vector<Entity> *enemy_;
  Entity *t_center_;
  Entity *t_house_;
  Entity *t_power_;
  Entity *t_army_;

};

#endif
