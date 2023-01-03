#include "bitboard.h"

int main(){
  init_dirBBmask();
  for(size_t i = D_LEFT; i<D_COUNT; i++){
    puts(arr_dirname[i]);
    printhexBB(arr_dirBBmask[i]);
  }

  word testBB[BBLEN] = {0};
  setBB(testBB, testBB, 17);
  setBB(testBB, testBB, 18);
  setBB(testBB, testBB, 61);
  
  printBB(testBB);

  containingBB(testBB, testBB, 3);
  return 0;
}

