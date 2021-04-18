#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "pytanie.h"
#include "generowanie.h"

#define KBLU  "\x1B[34m"
#define KRED  "\x1B[31m"
#define KWHT  "\x1B[37m"
#define KNRM  "\x1B[0m"

struct trojka {
  int first, second;
  char third;
};


int tab[30][30];
int plansza[30][30];
int pola_stale[30][30];
int wiersze[30][30];
int kolumny[30][30];
int kwadraty[30][30][30];
int wylosowane[30];
int kolejnosc[30];

void oznacz_pole(int x, int y, int z, int k) {
  tab[x][y] = z;
  wiersze[x][z] = 1;
  kolumny[y][z] = 1;
  kwadraty[x / k][y / k][z] = 1;
}


void oznacz_pola_stale(int wiel) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b  < wiel * wiel; b++) {
      if (plansza[a][b] != 0) {
        pola_stale[a][b] = 1;
      }
    }
  }
}

void czysc() {
  for(int a = 0; a < 30; a++) {
    wylosowane[a] = 0;
    kolejnosc[a] = 0;
    for (int b = 0; b < 30; b++) {
      tab[a][b] = 0;
      plansza[a][b] = 0;
      pola_stale[a][b] = 0;
      wiersze[a][b] = 0;
      kolumny[a][b] = 0;
      for (int c = 0; c < 30; c++) {
        kwadraty[a][b][c] = 0;
      }
    }
  }
}


void wypisuj_tab(int wiel) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      plansza[a][b] = tab[a][b];
    }
  }
}

void wypisuj_tab_odw(int wiel) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = plansza[a][b];
    }
  }
}

void przepisz (int wiel) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      wiersze[a][b + 1] = 0;
      kolumny[a][b + 1] = 0;
      tab[a][b] = 0;
      for (int c = 1; c <= wiel * wiel; c++) {
        kwadraty[a][b][c] = 0;
      }
    }
  }
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      if (plansza[a][b] != 0) {
        int x = plansza[a][b];
        tab[a][b] = x;
        kwadraty[a / wiel][b / wiel][x] = 1;
        wiersze[a][x] = 1;
        kolumny[b][x] = 1;
      }
    }
  }
}

int oznacz(int wiel) {
  int licz = 0;
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      if (plansza[a][b] == 0) {
        licz++;
      } else {
        pola_stale[a][b] = 1;
      }
    }
  }
  return licz;
}


int go(int a, int b, int wiel) {
  int z, v;
  if (tab[a][b] != 0) {
    if (b == wiel * wiel - 1) {
      if (a == wiel * wiel - 1) {
        return 1;
      }
      return go(a + 1, 0, wiel);
    } else {
      return go(a, b + 1, wiel);
    }
  }

  for (int x = 0; x < wiel * wiel; x++) {
    if (wiersze[a][kolejnosc[x]] == 0 && kolumny[b][kolejnosc[x]] == 0 &&
      kwadraty[a / wiel][b / wiel][kolejnosc[x]] == 0) {
      oznacz_pole (a, b, kolejnosc[x], wiel);
      tab[a][b] = kolejnosc[x];
      if (b == wiel * wiel - 1) {
        if (a == wiel * wiel - 1) {
          return 1;
        }
        v = 0;
        z = a + 1;
      } else {
        v = b + 1;
        z = a;
      }

      int m = go(z, v, wiel);
      if (m == 1) {
        return 1;
      }
      if (m == 0) {
        wiersze[a][kolejnosc[x]] = 0;
        kolumny[b][kolejnosc[x]] = 0;
        kwadraty[a / wiel][b / wiel][kolejnosc[x]] = 0;
        tab[a][b] = 0;
      }
    }
  }
  return 0;
}


int czy_dobry (int wiel, int wska[30][30]) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
      oznacz_pole(a, b, tab[a][b], wiel * wiel);
    }
  }
  int odp = go(0, 0, wiel);
  przepisz(wiel);
  return odp;
}

int test(int wiel, int wska[30][30]) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
      if (tab[a][b] != 0) {
        wiersze[a][tab[a][b]] = 1;
        kolumny[b][tab[a][b]] = 1;
        kwadraty[a / wiel][b / wiel][tab[a][b]] = 1;
        pola_stale[a][b] = 1;
      }
    }
  }
  int zwr = go (0, 0, wiel);
  wypisuj_tab(wiel);
  return zwr;
}

int podpowiedz (int wiel, int x, int y, int wska[30][30]) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
    }
  }
  go(0, 0, wiel);
  int odp = tab[x][y];
  przepisz(wiel);
  return odp;
}

int rozwiaz (int wiel, int wska[30][30]) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
    }
  }
  int odp = go(0, 0, wiel);
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      wska[a][b] = tab[a][b];
    }
  }
  return odp;
}

void generuj(int wiel, int po) {
  refresh();
  double liczba = gen_poz(po);
  clear();
  go(0, 0, wiel);
  srand(time(NULL));
  for (int a = 0; a < wiel * wiel * wiel * wiel * liczba;) {
    struct pair zwrot = losuj(wiel);
    int x = zwrot.first;
    int y = zwrot.second;
    int z = tab[x][y];
    if (z != 0) {
      tab[x][y] = 0;
      a++;
    }
  }
  wypisuj_tab(wiel);
  oznacz_pola_stale(wiel);
  przepisz(wiel);
}

void co(int co_robic) {
  srand(time(NULL));
  int czy_pusty = 0;
  int wiel = rozmiar();


  for (int a = 1; a <= 25; a++) {
    wylosowane[a] = a;
  }
  int a = wiel * wiel;

  while (a != 0) {
    int licz = (rand() % a) + 1;
    kolejnosc[wiel * wiel - a] = wylosowane[licz];
    for (int b = licz; b <= wiel * wiel; b++) {
      wylosowane[b] = wylosowane[b + 1];
    }
    a--;
  }


  czy_pusty = co_robic;
  if (czy_pusty == 2) {
    int po = poziom();
    generuj(wiel, po);
    wypisuj(wiel, (plansza), (pola_stale), 1);
  } else {
    wypisuj(wiel, (plansza), (pola_stale), 0);
  }

  czysc();
}
