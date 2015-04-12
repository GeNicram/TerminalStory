#include "entity.h"
#include <ncurses.h>
#include <math.h>
#include <string>
#include <vector>



std::vector<Entity>* Entity::enemy = NULL;
Entity* Entity::hero = NULL;

Entity* Entity::t_center = NULL;
Entity* Entity::t_house = NULL;
Entity* Entity::t_power = NULL;
Entity* Entity::t_army = NULL;


double dist(Entity* a, Entity* b)
{
//return sqrt((dX1 - dX0)*(dX1 - dX0) + (dY1 - dY0)*(dY1 - dY0));
  return sqrt((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
}

Entity::Entity()
  : type('?'),
    face_dir('r'),
    hp(10),
    attack_life(0),
    exp(0) {

}

void Entity::Draw(WINDOW* window) {
  int ix, iy;
  ix = floor(x);
  iy = floor(y);
  mvwprintw(window, iy, ix, "%c", type);
}

void Entity::DrawTown(WINDOW* window) {
  static std::string help_center[] = {
    "_________",
    "OOOOOOOOO",
    "O|OOOOO|O",
    "OHELPOOOO",
    "O|OOOOO|O",
    "_________",
    "O|SEARCHO",
    "OOOOOOOOO",
    "OOOOOOOOO",
    "OOO| |OOO"};

  static std::string houses[] = {
    "    O /O\\",
    "/O\\ | OOO",
    "OOO | OOO",
    "OOO      ",
    "/\\    /O\\",
    "OO /\\ OOO",
    "OO OO    ",
    "/O\\ /\\   ",
    "/\\ OOO OO",
    "OO OOO OO"};


  static std::string power_ups[] = {
    "|   ^^   |",
    "|  ^^^^  |",
    "| ^^^^^^ |",
    "|^^^^^^^^|",
    "| ^HHHH^ |",
    "|   HH   |",
    "|   HH ^ |",
    "|   HH   |",
    "| ^ HH   |",
    "|   HH   |"};
  static std::string army[] = {
    "    TT    ",
    "    TT    ",
    "    TT    ",
    "    TT    ",
    "    TT    ",
    "   TTTT   ",
    "TTTTTTTTTT",
    "TTTTTTTTTT",
    "    TT    ",
    "    TT    "};


  switch(type) {
    case 'c':
      mvwprintw(window, 1, 2, "Research");
      if(level < 10)
        mvwprintw(window, 2, 2, "a-up(%4i)", cost);
      for(int i = 0; i < level && i < 10; ++i)
        mvwprintw(window, 13-i, 2, "%s", help_center[9-i].c_str());
      break;
    case 'h':
      mvwprintw(window, 15, 2, "Houses");
      if(level < 10)
        mvwprintw(window, 16, 2, "z-up(%4i)", cost);
      for(int i = 0; i < level && i < 10; ++i)
        mvwprintw(window, 27-i, 2, "%s", houses[9-i].c_str());
      break;
    case 'p':
      mvwprintw(window, 1, 14, "Defence");
      if(level < 10)
        mvwprintw(window, 2, 14, "s-up(%4i)", cost);
      for(int i = 0; i < level && i < 10; ++i)
        mvwprintw(window, 13-i, 14, "%s", power_ups[9-i].c_str());
      break;
    case 'a':
      mvwprintw(window, 15, 14, "Army");
      if(level < 10)
        mvwprintw(window, 16, 14, "x-up(%4i)", cost);
      for(int i = 0; i < level && i < 10; ++i)
        mvwprintw(window, 27-i, 14, "%s", army[9-i].c_str());
      break;
  }
}

void Entity::DoHeroAttack() {
  attack_life = 0.5;
  // Search enemy
  for(auto it = enemy->begin(); it != enemy->end(); ++it) {
    if(dist(&(*it), this) < attack_size) {
      it->hp -= CountPower();
      it->attack_life = 0.5;
      exp++;
    }
  }
  if(exp > 10 + 3*level) {
    exp = 0;
    level++;
    speed += 0.2*level;
  }
}

void Entity::DoTownAttack() {
  float min = 10000;
  Entity* closest = NULL;
  for(auto it = enemy->begin(); it != enemy->end(); ++it) {
    if(it->x < min) {
      min = it->x;
      closest = &(*it);
    }
  }

  if(!closest || min > attack_size) {
    attack_life = 0;
    return;
  }

  closest->hp -= CountPower();
}

void Entity::DoTown(float _d) {
  attack_life -= _d;

  if(attack_life <= 0) {
    switch (type) {
      case 'h':
        hero->money += level;
        attack_life = 1;
        break;
      case 'p':
        hero->hp += (level > 0 ? 1 : 0) + 0.3*level;
        if(hero->hp > 100)
          hero->hp = 100;
        attack_life = 2 - 0.1*level;
        break;
      case 'a':
        attack_power = 1*level;
        attack_size = 40 + 2*level;
        attack_life = 1 - (float)t_power->level*0.05;
        DoTownAttack();
        break;
    }
  }
}

void Entity::DoHero(int g, float _d) {
  switch(g) {
    case KEY_LEFT:
      x -= speed * _d;
      break;
    case KEY_RIGHT:
      x += speed * _d;
      break;
    case KEY_UP:
      y -= speed * _d;
      break;
    case KEY_DOWN:
      y += speed * _d;
      break;
    case ' ':
      DoHeroAttack();
    default:
      break;
  }
  if(x < 26)
    x = 26;
  if(x > 98)
    x = 98;
  if(y < 1)
    y = 1;
  if(y > 28)
    y = 28;
}


void Entity::Do(float _d) {
  x -= speed * _d;
  if(x < 25) {
    hp = -1;
    hero->hp--;
  }

  if(attack_life > 0) {
    type = 'e';
    attack_life -= _d;
  } else
    type = 'E';
}
