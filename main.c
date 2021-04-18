#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include "sudoku.h"
#include "pytanie.h"
#include "przekatne.h"
#include "samurai.h"
//#include "niestandardowy.h"
//#include "przekatne.h"


int main() {
  setlocale(LC_ALL, "");
  initscr();
  use_default_colors();
  keypad(stdscr, TRUE);
  start_color();
  int co_robic = 1;
  //co_robic = standard ();
  co_robic = pyta();

  while (co_robic != 0) {
    int stan = standard();
    if (stan == 1) {
      co(co_robic);
    }
    if (stan == 2) {
      stan = niestandardowe();
      if (stan == 1) {
        co_sp(co_robic);
      } else {
        co_ss(co_robic);
      }
    }
    co_robic = pyta();
  }
  endwin();
  return 0;
}
