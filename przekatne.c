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
int przekatne[5][30];


int wie_ko_kwa_sp(int x, int y, int z, int k) {
  if (wiersze[x][z] == 1) {
    return 0;
  }
  if (kolumny[y][z] == 1) {
    return 0;
  }
  if (kwadraty[x / k][y / k][z] == 1) {
    return 0;
  }
  if (x == y && przekatne[0][z] == 1) {
    return 0;
  }
  if (x + y  + 1 == k * k && przekatne[1][z] == 1) {
    return 0;
  }
  return 1;
}

void oznacz_pole_sp(int x, int y, int z, int k) {
  tab[x][y] = z;
  wiersze[x][z] = 1;
  kolumny[y][z] = 1;
  kwadraty[x / k][y / k][z] = 1;
  if (x == y) {
    przekatne[0][z] = 1;
  }
  if (x + y  + 1 == k * k) {
    przekatne[1][z] = 1;
  }
}

void odznacz_pole_sp(int x, int y, int z, int k) {
  tab[x][y] = 0;
  wiersze[x][z] = 0;
  kolumny[y][z] = 0;
  kwadraty[x / k][y / k][z] = 0;
  if (x == y) {
    przekatne[0][z] = 0;
  }
  if (x + y  + 1 == k * k) {
    przekatne[1][z] = 0;
  }
}

void oznacz_pola_stale_sp(int wiel) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b  < wiel * wiel; b++) {
      if (plansza[a][b] != 0) {
        pola_stale[a][b] = 1;
      }
    }
  }
}

void czysc_sp() {
  for(int a = 0; a < 30; a++) {
    przekatne[0][a] = 0;
    przekatne[1][a] = 0;
    for (int b = 0; b < 30; b++) {
      tab[a][b] = 0;
      wiersze[a][b] = 0;
      kolumny[a][b] = 0;
      for (int c = 0; c < 30; c++) {
        kwadraty[a][b][c] = 0;
      }
    }
  }
}




void wypisuj_tab_sp(int wiel) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      plansza[a][b] = tab[a][b];
    }
  }
}

void przepisz_sp (int wiel) {
  for (int a = 0; a < wiel * wiel; a++) {
    przekatne[0][a] = 0;
    przekatne[1][a] = 0;
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
        if (a == b) {
          przekatne[0][x] = 1;
        }
        if (a + b + 1 == wiel * wiel) {
          przekatne[1][x] = 1;
        }
        tab[a][b] = x;
        kwadraty[a / wiel][b / wiel][x] = 1;
        wiersze[a][x] = 1;
        kolumny[b][x] = 1;
      }
    }
  }
}

int oznacz_sp (int wiel) {
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


int ustaw_przek(int a, int wiel) {
  for (int x = a; x < wiel * wiel; x++) {
    if (tab[x][x] != 0) {
      oznacz_pole_sp(x, x, tab[x][x], wiel);
    }
  }

  if (tab[a][a] != 0) {
    if (a == wiel * wiel - 1) {
      return 1;
    } else {
      return ustaw_przek(a + 1, wiel);
    }
  }

  for (int x = 0; x < wiel * wiel; x++) {
    int z = kolejnosc[x];
    int c = wie_ko_kwa_sp(a, a, z, wiel);
    if (c == 1) {
      oznacz_pole_sp(a, a, z, wiel);
      if (a == wiel * wiel - 1) {
       return 1;
      }
      int m = ustaw_przek(a + 1, wiel);
      if (m == 1) {
        return 1;
      }
      odznacz_pole_sp(a, a, z, wiel);
    }
  }
  return 0;
}


int go_sp(int a, int b, int wiel) {
  int z, v;
  if (tab[a][b] != 0) {
    if (b == wiel * wiel - 1) {
      if (a == wiel * wiel - 1) {
        return 1;
      }
      return go_sp(a + 1, 0, wiel);
    } else {
      return go_sp(a, b + 1, wiel);
    }
  }

  for (int x = 0; x < wiel * wiel; x++) {
    int c = wie_ko_kwa_sp(a, b, kolejnosc[x], wiel);
    if (c != 0) {
      oznacz_pole_sp (a, b, kolejnosc[x], wiel);
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

      int m = go_sp(z, v, wiel);
      if (m == 1) {
        return 1;
      }
      if (m == 0) {
        odznacz_pole_sp(a, b, kolejnosc[x], wiel);
      }
    }
  }
  return 0;
}



int czy_dobry_sp (int wiel, int wska[30][30]) {
  czysc_sp(wiel);
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
      if (tab[a][b] > 0) {
        oznacz_pole_sp(a, b, tab[a][b], wiel);
      }
    }
  }
  int odp = go_sp(0, 0, wiel);
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
      oznacz_pole_sp(a, b, tab[a][b], wiel);
    }
  }
  return odp;
}

int test_sp (int wiel, int wska[30][30]) {
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      if (wska[a][b] != 0) {
        oznacz_pole_sp(a, b, wska[a][b], wiel);
        pola_stale[a][b] = 1;
      }
    }
  }
  ustaw_przek(0, wiel);
  int zwr = go_sp (0, 0, wiel);
  //wypisuj_tab_sp(wiel);
  //return zwr;
  wypisuj_tab_sp (wiel);
}

int podpowiedz_sp (int wiel, int x, int y, int wska[30][30]) {
  czysc_sp(wiel);
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
      oznacz_pole_sp(a, b, tab[a][b], wiel);
    }
  }
  go_sp(0, 0, wiel);
  int odp = tab[x][y];
  przepisz_sp(wiel);
  return odp;
}

int rozwiaz_sp (int wiel, int wska[30][30]) {
  czysc_sp(wiel);
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      tab[a][b] = wska[a][b];
      oznacz_pole_sp(a, b, tab[a][b], wiel);
    }
  }
  int odp = go_sp(0, 0, wiel);
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      wska[a][b] = tab[a][b];
    }
  }
  return odp;
}

void generuj_sp(int wiel, int po) {
  srand(time(NULL));
  go_sp(0, 0, wiel);
  double liczba = gen_poz(po);

  for (int a = 0; a < wiel * wiel * wiel * wiel * liczba;) {
    struct pair zwrot = losuj(wiel);
    int x = zwrot.first;
    int y = zwrot.second;
    int z = tab[x][y];
    if (z != 0) {
      tab[x][y] = 0;
      odznacz_pole_sp(x, y, z, wiel);
      a++;
    }
  }
  wypisuj_tab_sp(wiel);
  oznacz_pola_stale_sp(wiel);
}

void co_sp (int co_robic) {
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

  for (int a = 0; a < wiel * wiel; a++) {
    tab[a][a] = kolejnosc[a];
    oznacz_pole_sp(a, a, kolejnosc[a], wiel);

  }

  czy_pusty = co_robic;
  if (czy_pusty == 2) {
    int po = poziom();
    generuj_sp(wiel, po);
    wypisuj(wiel, (tab), (pola_stale), 3);
  } else {
    wypisuj(wiel, (plansza), (pola_stale), 2);
  }

  czysc_sp();
  for (int a = 0; a < wiel * wiel; a++) {
    for (int b = 0; b < wiel * wiel; b++) {
      pola_stale[a][b] = 0;
      plansza[a][b] = 0;
    }
  }
}
