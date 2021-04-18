#ifndef SUDOKU_H
#define SUDOKU_H

void czysc();
int czy_dobry(int wiel, int wska[30][30]);
void wypisuj_tab (int wiel);
int test (int wiel, int wska[30][30]);
int go (int a, int b, int wiel);
void co (int co_robic);
void wczytaj (int wiel);
void przepisz (int wiel);
void oznacz (int wiel);
int gra (int x, int y, char znak, int wiel);
void generuj(int wiel, int po);
int podpowiedz(int wiel, int x, int y, int wska[30][30]);
int rozwiaz (int wiel, int wska[30][30]);

#endif
