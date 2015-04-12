#include "render.h"
#include "config.h"

#include <ncurses.h>
#include <unistd.h>

const int Render::screen_w = 100;
const int Render::screen_h = 32;

int Render::button_ = 0;

Render::Render()
  : hero_(NULL),
    enemy_(NULL),
    t_center_(NULL),
    t_house_(NULL),
    t_power_(NULL),
    t_army_(NULL) {
  initscr();
  keypad(stdscr, TRUE);
  noecho();

  start_color();
//  if(init_color(COLOR_GREEN, 0, 0, 100) == ERR)
//    printw("Error");

  init_pair(Grass, COLOR_BLACK, COLOR_GREEN);
  init_pair(Panel, COLOR_BLACK, COLOR_WHITE);
  init_pair(Enemy, COLOR_WHITE, COLOR_RED);
  init_pair(Message, COLOR_WHITE, COLOR_BLACK);

  refresh();

  wgame_ = CreateWindow(30, 100, 2, 0);
  wbar_ = CreateWindow(2, 100, 0, 0);
  Clear();
  wrefresh(wgame_);
  wrefresh(wbar_);
}

Render::~Render() {
  endwin();
}

void Render::Draw(float _d) {
  Clear();

  //Draw fence
  if(hero_->hp > 0)
    for(int i = 0; i < 30; ++i)
      mvwprintw(wgame_, i, 25, "#");

  DrawPanel(_d);

  if(hero_) {
    hero_->Draw(wgame_);
  }

  // Draw Entites
  if(enemy_) {
    attron(COLOR_PAIR(Enemy));
    for(auto it = enemy_->begin(); it != enemy_->end(); ++it) {
      it->Draw(wgame_);
    }
    attroff(COLOR_PAIR(Enemy));
  }

  if(t_center_)
    t_center_->DrawTown(wgame_);
  if(t_house_)
    t_house_->DrawTown(wgame_);
  if(t_power_)
    t_power_->DrawTown(wgame_);
  if(t_army_)
    t_army_->DrawTown(wgame_);

  wrefresh(wgame_);
  wrefresh(wbar_);
}

void Render::DrawPanel(float _d) {
  mvwprintw(wbar_, 0, 1, "Wall: %3i"
                         "  %7i $"
                         "  Level: %3i",
            hero_->hp, hero_->money, hero_->level);
  float rebuild = t_center_->level + t_house_->level +
                  t_power_->level + t_army_->level;
  rebuild = 10*rebuild/4;
  mvwprintw(wbar_, 1, 1, "Rebuid: %3.0f  "
      "Research: %2i Houses: %2i "
      "Defence: %2i Army: %2i",
            rebuild, t_center_->level,
            t_house_->level,
            t_power_->level,
            t_army_->level);

  mvwprintw(wbar_, 1, 89, "fps: %5f", _d);
}

void Render::Clear() {
  //erase();
  ClearGame();
  ClearPanel();
}

void Render::ClearGame() {
  wclear(wgame_);
  wbkgd(wgame_, COLOR_PAIR(Grass));
  box(wgame_, 0 , 0);
}

void Render::ClearPanel() {
  wbkgd(wbar_, COLOR_PAIR(Panel));
}

void Render::SayHello() {
  mvprintw(1,1,"Hello World » !!!\nNewLine");
  box(stdscr, 0, 0);
  refresh();
}

void Render::SayVersion(int _x, int _y) {
  mvprintw(_x, _y, "Ver. %d.%d", VERSION_MAJOR, VERSION_MINOR);
  refresh();
}

WINDOW* Render::CreateWindow(int height, int width, int starty, int startx) {
  WINDOW* local_win;

  local_win = newwin(height, width, starty, startx);

  wrefresh(local_win);

  return local_win;
}

void Render::Winner() {
  WINDOW* w = CreateWindow(10, 50, 5, 25);
  box(w, 0, 0);
  mvwprintw(w, 4, 10, "You WON!!!!");
  wrefresh(w);
  sleep(1);
  char buffer[256];
  getstr(buffer);
  mvwprintw(w, 7, 10, "Push anybutton to leave...");
  wrefresh(w);
  timeout(-1);
  getch();
}

void Render::Looser() {
  WINDOW* w = CreateWindow(10, 50, 5, 25);
  box(w, 0, 0);
  mvwprintw(w, 4, 10, "You looose :(");
  wrefresh(w);
  sleep(1);
  char buffer[256];
  getstr(buffer);
  mvwprintw(w, 7, 10, "If you want to try again");
  mvwprintw(w, 8, 10, "you have to rerun this game.");
  wrefresh(w);
  timeout(-1);
  getch();
}

void Render::Title() {

  WINDOW* w = CreateWindow(13, 50, 5, 25);
  box(w, 0, 0);
  mvwprintw(w, 2, 5, "Your home town ware destroyed.");
  mvwprintw(w, 3, 5, "Now, you have to rebuild your Town.");
  mvwprintw(w, 4, 5, "But...");
  mvwprintw(w, 5, 5, "Some kind of \"Ei\"vil forces what to");
  mvwprintw(w, 6, 5, "take your ground!");
  wrefresh(w);
  sleep(3);
  mvwprintw(w, 10, 20, "Push button");
  wrefresh(w);
  getch();

  wclear(w);

  box(w, 0, 0);
  mvwprintw(w, 1, 5, "Use your \"H\"-mobile and");
  mvwprintw(w, 2, 5, "aerial canon to protect town.");
  mvwprintw(w, 3, 5, "| arrows + space to shoot |");
  mvwprintw(w, 5, 5, "Order your people to rebuild town!");
  mvwprintw(w, 6, 5, "|a - research; s - defense|");
  mvwprintw(w, 7, 5, "|z - houses;   x - army   |");
  wrefresh(w);
  sleep(3);
  mvwprintw(w, 10, 20, "Push button");
  wrefresh(w);
  getch();
}
