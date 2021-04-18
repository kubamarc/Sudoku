#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "generowanie.h"

struct pair losuj(int wiel) {

  int a = rand() % (wiel * wiel);
  int b = rand() % (wiel * wiel);
  struct pair zwrot;
  //printf("%d %d\n", a, b);
  zwrot.first = a;
  zwrot.second = b;
  return zwrot;
}

struct pair losuj_ss() {

  int a = rand() % 21;
  int b = rand() % 21;
  struct pair zwrot;
  //printf("%d %d\n", a, b);
  zwrot.first = a;
  zwrot.second = b;
  return zwrot;
}

double gen_poz (int po) {
  if (po == 1) {
    return 0.57;
  }
  if (po == 2) {
    return 0.5;
  }
  if (po == 3) {
    return 0.45;
  }
}
