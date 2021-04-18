#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "pytanie.h"
#include "generowanie.h"

int tab[30][30];
int plansza[30][30];
int pola_stale[30][30];
int wiersze[30][30];
int kolumny[30][30];
int kwadraty[30][30][30];
int wylosowane[30];
int kolejnosc[30];

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"



void oznacz_poza_plansza_ss() {
  for (int a = 0; a < 23; a++) {
    for (int b = 0; b  < 23 ; b++) {
      if (tab[a][b] == 0) {
        tab[a][b] = -1;
      }
    }
  }
}

void oznacz_pola_stale_ss() {
  for (int a = 0; a < 21; a++) {
    for (int b = 0; b  < 21; b++) {
      if (plansza[a][b] > 0) {
        pola_stale[a][b] = 1;
      }
    }
  }
}

void czysc_ss() {
  for(int a = 0; a < 30; a++) {
    wylosowane[a] = 0;
    kolejnosc[a] = 0;
    for (int b = 0; b < 30; b++) {
      wiersze[a][b] = 0;
      kolumny[a][b] = 0;

      for (int c = 0; c < 30; c++) {
        kwadraty[a][b][c] = 0;
      }
    }
  }
}


void przepisz_ss () {
  for (int a = 0; a < 22; a++) {
    for (int b = 0; b < 22; b++) {
      plansza[a][b] = tab[a][b];
    }
  }
}

void przepisz_odw_ss () {
  for (int a = 0; a < 22; a++) {
    for (int b = 0; b < 22; b++) {
      tab[a][b] = plansza[a][b];
    }
  }
}

void kwad_ss (int x, int y) {
  for (int a = 0; a < 9; a++) {
    for (int b = 0; b < 9; b++) {
      if (tab[a + x][b + y] > 0) {
        int c = tab[a + x][b + y];
        kwadraty[a / 3][b / 3][c] = 1;
        wiersze[a][c] = 1;
        kolumny[b][c] = 1;
      }
    }
  }
}


int go_ss (int a, int b, int c, int d) {
  int z, v;
  if (tab[a][b] != 0) {
    if (b == d + 8) {
      if (a == c + 8) {
        return 1;
      }
      return go_ss(a + 1, d, c, d);
    } else {
      return go_ss(a, b + 1, c, d);
    }
  }

  for (int x = 0; x < 9; x++) {
    if (wiersze[a - c][kolejnosc[x]] == 0 && kolumny[b - d][kolejnosc[x]] == 0 &&
      kwadraty[(a - c) / 3][(b - d) / 3][kolejnosc[x]] == 0) {
      wiersze[a - c][kolejnosc[x]] = 1;
      kolumny[b - d][kolejnosc[x]] = 1;
      kwadraty[(a - c) / 3][(b - d) / 3][kolejnosc[x]] = 1;
      tab[a][b] = kolejnosc[x];
      if (b == d + 8) {
        if (a == c + 8) {
          return 1;
        }
        v = d;
        z = a + 1;
      } else {
        v = b + 1;
        z = a;
      }

      int m = go_ss(z, v, c, d);
      if (m == 1) {
        return 1;
      }
      if (m == 0) {
        wiersze[a - c][kolejnosc[x]] = 0;
        kolumny[b - d][kolejnosc[x]] = 0;
        kwadraty[(a - c) / 3][(b - d) / 3][kolejnosc[x]] = 0;
        tab[a][b] = 0;
      }
    }
  }
  return 0;
}

void permutacja_ss() {
  for (int a = 0; a < 9; a++) {
    wylosowane[a] = a + 1;
  }

  int a = 9;

  while (a != 0) {
    int licz = (rand() % a);
    kolejnosc[9 - a] = wylosowane[licz];
    for (int b = licz; b <= 9; b++) {
      wylosowane[b] = wylosowane[b + 1];
    }
    a--;
  }
}

int rozwiaz_ss() {
  kwad_ss(6, 6);
  permutacja_ss();
  int zwrot = go_ss (6, 6, 6, 6);
  czysc_ss();
  if (zwrot == 0) {
    return 0;
  }
  kwad_ss(0, 0);
  permutacja_ss();
  zwrot = go_ss(0, 0, 0, 0);
  czysc_ss();
  if (zwrot == 0) {
    return 0;
  }

  kwad_ss(12, 12);
  permutacja_ss();
  zwrot = go_ss(12, 12, 12, 12);
  czysc_ss();
  if (zwrot == 0) {
    return 0;
  }

  kwad_ss(12, 0);
  permutacja_ss();
  zwrot = go_ss(12, 0, 12, 0);
  czysc_ss();
  if (zwrot == 0) {
    return 0;
  }

  kwad_ss(0, 12);
  permutacja_ss();
  zwrot = go_ss(0, 12, 0, 12);
  czysc_ss();
  if (zwrot == 0) {
    return 0;
  }

  return 1;
}

int podpowiedz_ss(int x, int y) {
  getchar();
  int zwrot = rozwiaz_ss();
  int odp = tab[x][y];
  if (zwrot == 0) {
    return 0;
  }
  czysc_ss();
  plansza[x][y] = odp;

  return odp;
}


void rozwiazuj_ss () {
  for (int a = 0; a < 30; a++) {
    for (int b = 0; b < 30; b++) {
      //tab[a][b] = wska[a][b];
    }
  }

  rozwiaz_ss();

  for (int a = 0; a < 30; a++) {
    for (int b = 0; b < 30; b++) {
      plansza[a][b] = tab[a][b];
    }
  }
}



void generuj_ss (int po) {
  po++;
  double liczba = 0.5;
  rozwiaz_ss();
  oznacz_poza_plansza_ss();
  srand(time(NULL));
  for (int a = 0; a <  369 * liczba;) {
    przepisz_ss();
    int x = rand() % 22;
    int y = rand() % 22;
    int z = tab[x][y];


    if (z > 0) {
      tab[x][y] = 0;
      a++;
    }
  }
  przepisz_ss();
  oznacz_pola_stale_ss();
  rozwiaz_ss();
  wypisuj_ss((plansza), (pola_stale), 2);
}






void co_ss(int co_robic) {
  srand(time(NULL));
  int czy_pusty = 0;




  permutacja_ss();

  czy_pusty = co_robic;
  if (czy_pusty == 2) {
    int po = 1;
    generuj_ss(po);
  } else {
  }

  czysc_ss();
}
