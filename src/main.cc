#include <ctime>
#include <chrono>
#include <ncurses.h>
#include <math.h>
#include <stdlib.h> 
#include <string.h>
#include "render.h"
#include "entity.h"

bool increase_level = false;
Entity hero;

Entity SpawnEnemy(int level) {
  Entity en;
  en.hp = level;
  en.level = level;
  en.speed = 2*(float)level/1.0f;
  en.type = 'E';
  en.x = 100 + rand() % 5;
  en.y = 2.0f + rand() % 26;
  return en;
}

int TimerSpawner(float _d) {
  static float passed = 0;
  static int level = 0;
  passed += _d;

  switch(level) {
    case 5:
      if(passed > 3 - (float)level/80) {
        passed = 0;
        level = 0;
        increase_level = true;
        return 0;
      }
      break;
    case 4:
      if(passed > 4 - (float)level/80) {
        passed = 0;
        level = 5;
        return 10;
      }
      break;
    case 3:
      if(passed > 2 - (float)level/80) {
        passed = 0;
        level = 4;
        return 6;
      }
      break;
    case 2:
      if(passed > 5 - (float)level/80) {
        passed = 0;
        level = 3;
        return 4;
      }
      break;
    case 1:
      if(passed > 5 - (float)level/80) {
        passed = 0;
        level = 2;
        return 3;
      }
      break;
    case 0:
      if(passed > 1 - (float)level/100) {
        passed = 0;
        level = 1;
        return 1;
      }
      break;
  }

  return 0;
}

void Buy(Entity* b) {
  if(hero.money >= b->cost && b->level < 10) {
    hero.money -= b->cost;
    b->cost = 2*b->cost + 5;
    b->level++;
  }
}

bool CheckState() {
  
}


int main(int _args, char** _argv) {
  Render renderer;

  float passed;

  int g;

  renderer.Title();

  hero.hp = 100;
  hero.speed = 20;
  hero.type = 'H';
  hero.x = 26;
  hero.y = 15;
  hero.attack_size = 3;
  hero.attack_power = 2;
  hero.money = 10;
  renderer.SetHero(&hero);
  Entity::hero = &hero;

  Entity t_center;
  t_center.level = 0;
  t_center.type = 'c';
  t_center.cost = 1;
  Entity::t_center = &t_center;
  Entity t_house;
  t_house.level = 0;
  t_house.type = 'h';
  t_house.cost = 1;
  Entity::t_house = &t_house;
  Entity t_power;
  t_power.level = 0;
  t_power.type = 'p';
  t_power.cost = 1;
  Entity::t_power = &t_power;
  Entity t_army;
  t_army.level = 0;
  t_army.type = 'a';
  t_army.cost = 1;
  Entity::t_army = &t_army;

  renderer.SetTown(&t_center, &t_house, &t_power, &t_army);

  std::vector<Entity> enemy;
  int level = 1;
  renderer.SetEnemy(&enemy);
  Entity::enemy = &enemy;

  if(_args >= 2 && strcmp(_argv[1], "loose") == 0)
    hero.hp = 1;
  if(_args >= 2 && strcmp(_argv[1], "win") == 0)
    hero.money = 9999999;


  while(g != 27 ) {
    auto start = std::chrono::high_resolution_clock::now();

    switch(g) {
      case KEY_LEFT:
      case KEY_RIGHT:
      case KEY_UP:
      case KEY_DOWN:
      case ' ':
        Render::button_ = g;
        hero.DoHero(g, passed);
        break;
      case 'z':
        Buy(&t_house);
        break;
      case 'a':
        Buy(&t_center);
        break;
      case 'x':
        Buy(&t_army);
        break;
      case 's':
        Buy(&t_power);
        break;
      case 'Y':
        t_center.level++;
        t_house.level++;
        t_power.level++;
        t_army.level++;
        break;
      case 'y':
        t_center.level--;
        t_house.level--;
        t_power.level--;
        t_army.level--;
        break;
      default:
        Render::button_ = 0;
    }

    // Do game logic
    int spawn = TimerSpawner(passed);
    if(increase_level) {
      level++;
      increase_level = false;
    }
    
    for(int i = 0; i < spawn; ++i) {
      enemy.push_back(SpawnEnemy(level));
    }

    for(auto it = enemy.begin(); it != enemy.end(); /*++it*/) {
      it->Do(passed);
      if(it->hp < 0) {
        hero.money += it->level;
        it = enemy.erase(it);
      }
      else
        ++it;
    }

    t_center.DoTown(passed);
    t_army.DoTown(passed);
    t_house.DoTown(passed);
    t_power.DoTown(passed);


    renderer.Draw(passed);
    timeout(100);
    g = getch();
    passed = std::chrono::duration<float>(
              std::chrono::high_resolution_clock::now() - start).count();
    if(t_center.level + t_army.level + t_house.level + t_power.level >= 40) {
      renderer.Winner();
      break;
    } else if(hero.hp <= 0) {
      renderer.Looser();
      break;
    }
  }

  return 0;
}
