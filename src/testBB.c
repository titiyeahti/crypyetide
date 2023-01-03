#include "bitboard.h"

int main(){
  word line = (1 << 12) - 1;
  word aBB[BBLEN] = {0};
  word bBB[BBLEN] = {0};
  aBB[0] = line;
  
  printBB(aBB);

  for(size_t i = 0; i < BBLEN; i++){
    shiftBB(aBB, bBB, 24);
    unionBB(aBB, bBB, aBB);
    printBB(aBB);
  }

  printhexBB(aBB);
  negBB(aBB, aBB);
  printBB(aBB);
  printhexBB(aBB);
  setBB(aBB, aBB, 107);
  fixlastwordBB(aBB, aBB);
  printhexBB(aBB);
  return 0;
}
