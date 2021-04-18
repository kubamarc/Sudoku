#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <string.h>
#include "sudoku.h"
#include "przekatne.h"
#include "samurai.h"

struct pair {
  int first, second;
};

int plansza[30][30];
int pola_stale[30][30];
int cofnij[10000][3];

void wyczysc() {
  for (int a = 0; a < 30; a++) {
    for (int b = 0; b < 30; b++) {
      plansza[a][b] = 0;
      pola_stale[a][b] = 0;
    }
  }
  for (int a = 0; a < 10000; a++) {
    for (int b = 0; a < 3; a++) {
      cofnij[a][b];
    }
  }
}

struct pair wspolrzedne(int x, int y, int wiel) {
  int a, b;
  a = x * 3 + x / wiel;
  b = y + y / wiel;
  struct pair zwrot;
  zwrot.first = a;
  zwrot.second = b;
  return zwrot;
}

struct pair wspolrzedne_ss(int x, int y) {
  int a, b;
  a = x * 3 + x / 3;
  b = y + y / 3;
  struct pair zwrot;
  zwrot.first = a;
  zwrot.second = b;
  return zwrot;
}

void help() {
  clear();
  refresh();
  int row, col;
  getmaxyx(stdscr,row,col);
  char litera = 'a';
  while (litera != 'q') {
    printw("Aby poruszać się po planszy, używaj strzałek\n");
    printw("Aby wpisać liczbę w pole, należy \"wejść\" na pole, i wpisać liczbę(gdy użytkownik gra w sudoku 16x16 i wpisze 1, program będzie czekał na następny znak\n");
    printw("Jeżeli będzie to liczba z zakresu [0, 6], program wpisze w dane pole odpowiednią liczbę z zakresu [10, 16]. Dla liczby od 7 do 9\n");
    printw("wpisze tę liczbę. Jeżeli gracz wciśnie strzałkę, program wpisze 1)\n");
    printw("Dostępne są również następujące opcje: \n");
    printw("Po wciśnięciu \"r\" program poda rozwiązanie sudoku\n");
    printw("Po wciśnięciu \"p\" program wpisze w pole, w którym się znajduje odpowiednią liczbę(jeżeli takowa istnieje)\n");
    printw("Aby zakończyć grę należy wcisnąć \"e\", bądź \"k\"\n");
    mvprintw(row - 2, 0, "Aby wyjść wciśnij \"q\"\n");
    mvprintw(row - 1, 0, "Autorem programu jest Jakub Marcinkowski. Powodzenia w grze!");
    litera = getch();
  }
}



void wypisuj(int wiel, int wska1[30][30], int wska2[30][30], int tryb) {
  int licz = 0;
  int wyp = 0;
  int odp = -1;
  int end = 1;
  int nwjtn = 0;
  int zwr = 3;
  int row, col;
  int ruch;
  getmaxyx(stdscr,row,col);
  keypad(stdscr, TRUE);
  int x = 3, y = 3;
  init_pair(1, COLOR_BLUE, -1);
  init_pair(2, COLOR_RED, -1);
  init_pair(3, COLOR_WHITE, -1);
  init_pair(4, COLOR_BLUE, COLOR_WHITE);
  init_pair(5, COLOR_RED, COLOR_WHITE);
  init_pair(6, COLOR_GREEN, COLOR_WHITE);
  init_pair(7, COLOR_RED, COLOR_BLACK);
  init_pair(8, COLOR_BLUE, COLOR_BLACK);
  init_pair(9, COLOR_WHITE, COLOR_BLACK);
  clear();
  refresh();
  int puste = 1;

  while (end != 0) {
    for (int a = 0; a < wiel * wiel; a++) {
      for (int b = 0; b < wiel* wiel; b++) {
        plansza[a][b] = wska1[a][b];
        pola_stale[a][b] = wska2[a][b];
      }
    }
    clear();
    puste = 0;
    int poczx, poczy;
    poczx = (row - wiel * wiel) / 2;
    poczy = (col - 3 * wiel * wiel) / 2;
    move(poczy, poczx);
    mvprintw(poczy, poczx, " ");
    printw("\u2554");
    for (int a = 0; a <= wiel * wiel; a++) {
      if (a % wiel == 0) {
        if (a != 0) {
          printw("\n");
        }
        if (a != wiel * wiel && a % wiel != 0) {
          printw("\u2560");
        }
        if (a % wiel == 0 && a != 0 && a != wiel * wiel) {
          printw("\u2560");
        }
        if (a == wiel * wiel) {
          printw("\u255A");
        }
        int k = wiel * wiel * 3 + wiel;
        char p = 126;
        for (int b = 1; b <= k; b++) {
          if (b % (3 * wiel + 1) == 1 && a != 0 && b != 1 && a != wiel * wiel) {
            printw("\u256C");
          }
          if (b % (3 * wiel + 1) == 1 && a == 0 && b != 1) {
            printw("\u2566");
          }
          if (b % (3 * wiel + 1) == 1 && a == wiel * wiel && b != 1) {
            printw("\u2569");
          }
          printw("\u2550");
        }
        if (a == 0) {
          printw("\u2557");
        }
        if (a == wiel * wiel) {
          printw("\u255D");
        }
        if (a != 0 && a != wiel * wiel) {
          printw("\u2563");
        }
      }
      printw("\n");
      if (a != wiel * wiel) {
        for (int b = 0; b <= wiel * wiel; b++) {
          if (b % wiel == 0) {
            printw("\u2551 ");
          }
          if (b != wiel * wiel) {
            if (pola_stale[a][b] == 0) {
              if (plansza[a][b] != 0) {
                if (x == a && y == b) {
                  attron(COLOR_PAIR(4));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(4));
                } else {
                  attron(COLOR_PAIR(1));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(1));
                }
              } else {
                if (x == a && y == b) {
                  attron(COLOR_PAIR(6));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(6));
                } else {
                  attron(COLOR_PAIR(3));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(3));
                }
              }
            } else {
              if (x == a && y == b) {
                attron(COLOR_PAIR(5));
                printw("%2d ", plansza[a][b]);
                attroff(COLOR_PAIR(5));
              } else {
                attron(COLOR_PAIR(2));
                printw("%2d ", plansza[a][b]);
                attroff(COLOR_PAIR(2));
              }
            }
          }
        }
      }
    }
    mvprintw(row - 1, 0, "Naciśnij \"h\", aby zobaczyć dostępne opcje");




    if (nwjtn == 0) {
      ruch = getch();
    }

    nwjtn = 0;
    int a = x;
    int b = y;
    if (ruch == KEY_UP) {
      if (x == 0) {
        x = wiel * wiel - 1;
      } else {
        x -= 1;
      }
      struct pair zwrot = wspolrzedne(y, x, wiel);
      a = zwrot.first;
      b = zwrot.second;
      move(b + poczy, a + poczx);
      refresh();
    }

    if (ruch == KEY_LEFT) {
      if (y == 0) {
        y = wiel * wiel - 1;
      } else {
        y -= 1;
      }
      struct pair zwrot = wspolrzedne(y, x, wiel);
      a = zwrot.first;
      b = zwrot.second;
      move(b + poczy, a + poczx);
      refresh();
    }
    if (ruch == KEY_DOWN) {
      if (x == wiel * wiel - 1) {
        x = 0;
      } else {
        x += 1;
      }
      struct pair zwrot = wspolrzedne(y, x, wiel);
      a = zwrot.first;
      b = zwrot.second;
      move(b + poczy, a + poczx);
      refresh();
    }
    if (ruch == KEY_RIGHT) {
      if (y == wiel * wiel - 1) {
        y = 0;
      } else {
        y += 1;
      }
      struct pair zwrot = wspolrzedne(y, x, wiel);
      a = zwrot.first;
      b = zwrot.second;
      move(b + poczy, a + poczx);
      refresh();
      }

      if (tryb % 2 == 0) {
        if (((wiel != 4 && ruch - '0' >= 0 && ruch - '0' < 10) || (wiel == 4 && ruch - '0' != 1 && ruch - '0' >= 0 && ruch - '0' < 10)) && pola_stale[x][y] == 0) {
          int z = plansza[x][y];
          cofnij[licz][0] = x;
          cofnij[licz][1] = y;
          cofnij[licz][2] = z;
          licz++;
          plansza[x][y] = ruch - '0';
        }
        if (wiel == 4 && ruch - '0' == 1 && pola_stale[x][y] == 0) {
          ruch = getch();
          if (ruch - '0' >= 0 && ruch - '0' < 7) {
            zwr = 1;
            int z = plansza[x][y];
            cofnij[licz][0] = x;
            cofnij[licz][1] = y;
            cofnij[licz][2] = z;
            licz++;
            plansza[x][y] = ruch - '0';
            plansza[x][y] += 10;

          } else {
            int z = plansza[x][y];
            cofnij[licz][0] = x;
            cofnij[licz][1] = y;
            cofnij[licz][2] = z;
            licz++;
            plansza[x][y] = 1;
            nwjtn = 1;
          }
        }
        if (ruch == 'r') {
          if (tryb == 0) {
            test(wiel, (plansza));
          } else {
            test_sp(wiel, (plansza));
          }
        }
      } else {
        if (((wiel != 4 && ruch - '0' >= 0 && ruch - '0' < 10) || (wiel == 4 && ruch - '0' != 1 && ruch - '0' >= 0 && ruch - '0' < 10)) && pola_stale[x][y] == 0) {
          int z = plansza[x][y];
          cofnij[licz][0] = x;
          cofnij[licz][1] = y;
          cofnij[licz][2] = z;
          licz++;
          plansza[x][y] = ruch - '0';
        }
        if (wiel == 4 && ruch - '0' == 1 && pola_stale[x][y] == 0) {
          ruch = getch();
          if (ruch - '0' >= 0 && ruch - '0' < 7) {
            int z = plansza[x][y];
            cofnij[licz][0] = x;
            cofnij[licz][1] = y;
            cofnij[licz][2] = z;
            licz++;
            plansza[x][y] = ruch - '0';
            plansza[x][y] += 10;
          } else {
            int z = plansza[x][y];
            cofnij[licz][0] = x;
            cofnij[licz][1] = y;
            cofnij[licz][2] = z;
            licz++;
            plansza[x][y] = 1;
            nwjtn = 1;
          }
        }
      /*  if (ruch == 'c') {
          if (tryb == 1) {
            odp = czy_dobry(wiel, (plansza));
            odp = czy_dobry(wiel, (plansza));
          }
          if (tryb == 3) {
            odp = czy_dobry_sp(wiel, (plansza));
            odp = czy_dobry_sp(wiel, (plansza));
          }
        }*/
        if (ruch == 'p') {
          if (tryb == 1) {
            licz = podpowiedz(wiel, x, y, (plansza));
            licz = podpowiedz(wiel, x, y, (plansza));
            int z = plansza[x][y];
            cofnij[licz][0] = x;
            cofnij[licz][1] = y;
            cofnij[licz][2] = z;
            licz++;
            plansza[x][y] = licz;
          }
          licz = podpowiedz_sp(wiel, x, y, (plansza));
          licz = podpowiedz_sp(wiel, x, y, (plansza));
          plansza[x][y] = licz;
        }
        if (ruch == 'r') {
          if (tryb == 1) {
            zwr = rozwiaz(wiel, (plansza));
          }
          if (tryb == 3) {
            zwr = rozwiaz_sp(wiel, (plansza));
          }
        }
      }
      if (ruch == 'b' && licz > 0) {
        licz--;
        plansza[cofnij[licz][0]][cofnij[licz][1]] = cofnij[licz][2];
      }
    if (ruch == 'k' || ruch == 'e') {
      end = 0;
      wyczysc();
    }
    if (ruch == 'h') {
      help();
    }
  }
  refresh();
  return;
}



void wypisuj_ss(int wska1[30][30], int wska2[30][30], int tryb) {
  int licz = 0;
  int wyp = 0;
  int odp = -1;
  int end = 1;
  int nwjtn = 0;
  int zwr = 3;
  int row, col;
  int ruch;
  getmaxyx(stdscr,row,col);
  keypad(stdscr, TRUE);
  int x = 3, y = 3;
  init_pair(1, COLOR_BLUE, -1);
  init_pair(2, COLOR_RED, -1);
  init_pair(3, COLOR_WHITE, -1);
  init_pair(4, COLOR_BLUE, COLOR_WHITE);
  init_pair(5, COLOR_RED, COLOR_WHITE);
  init_pair(6, COLOR_GREEN, COLOR_WHITE);
  init_pair(7, COLOR_RED, COLOR_BLACK);
  init_pair(8, COLOR_BLUE, COLOR_BLACK);
  init_pair(9, COLOR_WHITE, COLOR_BLACK);
  clear();
  refresh();
  int puste = 1;

  for (int a = 0; a < 22; a++) {
    for (int b = 0; b < 22; b++) {
      plansza[a][b] = wska1[a][b];
      pola_stale[a][b] = wska2[a][b];
    }
  }

  while (end != 0) {
    clear();
    int poczx, poczy;
    poczx = (row - 21) / 2;
    poczy = (col - 3 * 21) / 2;
    mvprintw(poczy, poczx, " ");
    for (int a = 0; a <= 21; a++) {
      if (a % 3 == 0) {
        if (a != 0) {
          printw("\n");
        }
        for (int b = 0; b < 71; b++) {
          if (b == 0 && a == 0) {
            printw("\u2554");
          }
          if (a == 21 && b == 0) {
            printw("\u255A");
          }
          if (a == 21 && b == 70) {
            printw("\u255D");
          }
          if (a == 21 && b != 0 && b != 70 && b % 10 == 0) {
            printw("\u2569");
          }
          if (b % 10 == 0 && a > 0 && a < 21 && b != 0 && b != 70) {
            printw("\u256C");
          }
          if (b == 70 && a > 0 && a < 21) {
            printw("\u2563");
          }
          if (a % 3 == 0 && a > 0 && b == 0 && a != 21) {
            printw("\u2560");
          }
          if (a == 0 && b != 0 && b % 10 == 0 && b != 70) {
            printw("\u2566");
          }
          if (b != 0 && b % 10 != 0 && b != 70) {
            printw("\u2550");
          }
          if (a == 0 && b == 70) {
            printw("\u2557");
          }
        }
      }
      printw("\n");
      if (a != 21) {
        for (int b = 0; b <= 21; b++) {
          if (b % 3 == 0) {
            printw("\u2551");
          }
          if (b != 21) {
            if (pola_stale[a][b] == 0) {
              if (plansza[a][b] > 0) {
                if (x == a && y == b) {
                  attron(COLOR_PAIR(4));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(4));
                } else {
                  attron(COLOR_PAIR(1));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(1));
                }
              }
              if (plansza[a][b] == 0){
                if (x == a && y == b) {
                  attron(COLOR_PAIR(6));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(6));
                } else {
                  attron(COLOR_PAIR(3));
                  printw("%2d ", plansza[a][b]);
                  attroff(COLOR_PAIR(3));
                }
              }
              if (plansza[a][b] < 0) {
                printw("   ");
              }
            } else {
              if (x == a && y == b) {
                attron(COLOR_PAIR(5));
                printw("%2d ", plansza[a][b]);
                attroff(COLOR_PAIR(5));
              } else {
                attron(COLOR_PAIR(2));
                printw("%2d ", plansza[a][b]);
                attroff(COLOR_PAIR(2));
              }
            }
          }
        }
      }
    }
    mvprintw(row - 1, 0, "Naciśnij \"h\", aby zobaczyć dostępne opcje");


    ruch = getch();
    int a = x;
    int b = y;
    if (ruch == KEY_UP) {
      if ((x != 0 && plansza[x - 1][y] == -1) || x == 0) {
        x = 21 - x - 1;
      } else {
        x -= 1;
      }

      struct pair zwrot = wspolrzedne_ss(y, x);
      a = zwrot.first;
      b = zwrot.second;
      //move(b + poczy, a + poczx);
      refresh();
    }

    if (ruch == KEY_LEFT) {
      if ((y != 0 && plansza[x][y - 1] == -1)|| y == 0) {
        y = 21 - y - 1;
      } else {
        y -= 1;
      }

      struct pair zwrot = wspolrzedne_ss(y, x);
      a = zwrot.first;
      b = zwrot.second;
      //move(b + poczy, a + poczx);
      refresh();
    }

    if (ruch == KEY_DOWN) {
      if (plansza[x + 1][y] == -1) {
        x = 21 - x - 1;
      } else {
        x += 1;
      }
      struct pair zwrot = wspolrzedne_ss(y, x);
      a = zwrot.first;
      b = zwrot.second;
      //move(b + poczy, a + poczx);
      refresh();
    }

    if (ruch == KEY_RIGHT) {
      if (plansza[x][y + 1] == -1) {
        y = 21 - y - 1;
      } else {
        y += 1;
      }
      struct pair zwrot = wspolrzedne_ss(y, x);
      a = zwrot.first;
      b = zwrot.second;
      //move(b + poczy, a + poczx);
      refresh();
    }

    if ((ruch - '0' >= 0 && ruch - '0' < 10) && pola_stale[x][y] == 0) {
      int z = plansza[x][y];
      cofnij[licz][0] = x;
      cofnij[licz][1] = y;
      cofnij[licz][2] = z;
      licz++;
      plansza[x][y] = ruch - '0';
    }

    if (ruch == 'p') {
      int z = plansza[x][y];
      cofnij[licz][0] = x;
      cofnij[licz][1] = y;
      cofnij[licz][2] = z;
      licz++;
      int pod = podpowiedz_ss(x, y, (plansza));
      plansza[x][y] = pod;
    }

    if (ruch == 'r') {
      rozwiazuj_ss((plansza));
    }

    if (ruch == 'b' && licz > 0) {
      licz--;
      plansza[cofnij[licz][0]][cofnij[licz][1]] = cofnij[licz][2];
    }

    if (ruch == 'k' || ruch == 'e') {
      end = 0;
      wyczysc();
    }
    if (ruch == 'h') {
      help();
    }
  }
  refresh();
  return;
}




int standard () {
  int liczba = 0;
  while (liczba < 1 || liczba > 2) {
    clear ();
    refresh();
    printw("Wybierz tryb: ");
    printw("Jeżeli chcesz grać w standardowe sudoku, wpisz 1\n");
    printw("Jeżeli chcesz grać w niestandardowe sudoku wpisz 2\n");
    refresh();
    char pom = getch();
    if (pom - 'e' == 0) {
      return -1;
    }
    liczba = pom - '0';
  }
  return liczba;
}

int pyta() {
  int liczba = 3;
  while (liczba > 2 || liczba < 0) {
    clear ();
    refresh();
    printw("Wybierz, co chcesz robić\n");
    printw("Jeżeli chcesz zakończyć zabawę z sudoku, wpisz \"0\"\n");
    printw("Jeżeli chcesz zobaczyć rozwiązanie swojej zagadki wpisz \"1\"\n");
    printw("Jeżeli chcesz spróbować rozwiązać zagadkę sudoku wpisz \"2\"\n");
    char pom = getch();
    liczba = pom - '0';
    refresh();
  }
  return liczba;
}





int rozmiar () {
  int wiel = 5;
  while (wiel > 4 || wiel < 2) {
    clear ();
    refresh();
    printw("Podaj wielkość planszy\n");
    printw("Jeżeli 4x4 wpisz 2\n");
    printw("Jeżeli 9x9 wpisz 3\n");
    printw("Jeżeli 16x16 wpisz 4\n");
    char pom = getch();
    wiel = pom - '0';
    if (pom == 'e') {
      return -1;
    }
    refresh();
  }
  return wiel;
}

int poziom () {
  int liczba = 4;
  while (liczba > 3) {
    clear ();
    refresh();
    printw("Wybierz poziom trudności\n");
    printw("Najtrudniejszy jest poziom 3, najłatwiejszy poziom 1 \n");
    char pom = getch();
    liczba = pom - '0';
    if (pom == 'e') {
      return -1;
    }
    refresh();
  }
  return liczba;
}

int niestandardowe () {
  int liczba = 3;
  while (liczba > 2 || liczba < 1) {
    clear ();
    refresh();
    printw("Wybierz niestandardowe sudoku, w które chcesz zagrać\n");
    printw("Jeżeli chcesz zagrać w sudoku z różnymi liczbami w głównych przekątnych, wpisz \"1\"\n");
    printw("Jeżeli chcesz zagrać w samurai sudoku,  wpisz \"2\"\n");
    char pom = getch();
    liczba = pom - '0';
    if (pom == 'e') {
      return -1;
    }
    refresh();
  }
  return liczba;
}
