#include "cryptide.h"

int fctest(size_t n, slot tab[n]){
  printf("%ld\n", sizeof(tab));
  return 0;
}

int neigh_index(size_t index, uint8_t neigh[6]){
  size_t row, col, delta;
  row = index / WIDTH;
  col = index % WIDTH;
  delta = index & 1;

  neigh[0] = (col < WIDTH - 1) ? index + 1 : UINT8_MAX;
  neigh[1] = (col > 0) ? index - 1 : UINT8_MAX;
  neigh[2] = ((row < HEIGHT - 1) && delta) ? index + WIDTH - 1 + delta : UINT8_MAX;
  neigh[3] = ((row < HEIGHT - 1) && delta) ? index + delta : UINT8_MAX;
  neigh[4] = (row < WIDTH - 1) ? index + 1 : UINT8_MAX;
  neigh[5] = (row < WIDTH - 1) ? index + 1 : UINT8_MAX;


int main(void){
  slot test[WIDTH*HEIGHT];
  printf("%ld, %ld\n", sizeof(slot), sizeof(test));
  fctest(WIDTH*HEIGHT, test);
  return EXIT_SUCCESS;
}
