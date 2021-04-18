#ifndef PRZEKATNE_H
#define PRZEKATNE_H

void czysc_sp();
void wypisuj_tab_sp (int wiel);
int test_sp (int wiel, int wska[30][30]);
int go_sp (int a, int b, int wiel);
void co_sp (int co_robic);
void wczytaj_sp (int wiel);
void przepisz_sp (int wiel);
void oznacz_sp (int wiel);
int gra_sp (int x, int y, char znak, int wiel);
void generuj_sp (int wiel, int po);
int czy_dobry_sp(int wiel, int wska[30][30]);
int podpowiedz_sp(int wiel, int x, int y, int wska[30][30]);
int rozwiaz_sp (int wiel, int wska[30][30]);

#endif
