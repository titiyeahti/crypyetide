#include "bitboard.h"

int main(){
  word aBB[BBLEN]={0};
  word bBB[BBLEN]={0};
  
  for(size_t i=D_LEFT; i<D_COUNT; i++)
    printf("direction[%u] = %d, %s\n", i, arr_dir[i], arr_dirname[i]);

  putc('\n', stdout);

  setBB(aBB, aBB, 17);
  setBB(aBB, aBB, 76);
  printBB(aBB);
  for(size_t i=D_LEFT; i<D_COUNT; i++){
    shiftBB(aBB, bBB, arr_dir[i]);
    unionBB(aBB, bBB, bBB);
    puts(arr_dirname[i]);
    putc('\n', stdout);
    printBB(bBB);
  }

  return 0;
}
