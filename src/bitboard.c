#include "bitboard.h"

#define DEF(E,V,S) V
const int const arr_dir[D_COUNT] = {
#include "directions.def"
};
#undef DEF

#define DEF(E,V,S) S
const char* const arr_dirname[D_COUNT] = {
#include "directions.def"
};
#undef DEF

word arr_dirBBmask[D_COUNT][BBLEN] = {0};

int shiftBB(word inBB[], word outBB[], int value){
  return (value < 0) ? shiftrightBB(inBB, outBB, (size_t) (-value)) :
      shiftleftBB(inBB, outBB, (size_t) value);
}

int shiftleftBB(word inBB[], word outBB[], size_t value){
  assert(value < WORDLEN);
  word tmp = 0;
  word ret = 0;
  for(size_t i = 0; i < BBLEN; i++){
    tmp = inBB[i];
    outBB[i] = ret + (inBB[i] << value);
    ret = tmp >> (WORDLEN-value);
  }

  fixlastwordBB(outBB, outBB);
  return 0;
}

int shiftrightBB(word inBB[], word outBB[], size_t value){
  assert(value < WORDLEN);
  word tmp = 0;
  word ret = 0;
  for(size_t i = BBLEN-1; i < BBLEN; i--){
    tmp = inBB[i];
    outBB[i] = ret + (inBB[i] >> value);
    ret = tmp << (WORDLEN-value);
  }

  return 0;
}

int unionBB(word aBB[], word bBB[], word outBB[]){
  for(size_t i = 0; i < BBLEN; i++)
    outBB[i] = aBB[i] | bBB[i];

  return 0;
}

int interBB(word aBB[], word bBB[], word outBB[]){
  for(size_t i = 0; i < BBLEN; i++)
    outBB[i] = aBB[i] & bBB[i];

  return 0;
}

int xorBB(word aBB[], word bBB[], word outBB[]){
  for(size_t i = 0; i < BBLEN; i++)
    outBB[i] = aBB[i] ^ bBB[i];

  return 0;
}

int negBB(word inBB[], word outBB[]){
  for(size_t i = 0; i < BBLEN; i++)
    outBB[i] = ~inBB[i];

  fixlastwordBB(outBB, outBB);
  return 0;
}

int copyBB(word destBB[], word srcBB[]){
  if(destBB == srcBB) return 0;
  return !memcpy(destBB, srcBB, sizeof(word)*BBLEN);
}

int setBB(word inBB[], word outBB[], size_t index){
  assert(index < LENGTH);
  outBB[index >> WORDPOW] = inBB[index >> WORDPOW] | 
    (1U << (index & (WORDLEN-1)));

  return 0;
}

int unsetBB(word inBB[], word outBB[], size_t index){
  assert(index < LENGTH);
  outBB[index >> WORDPOW] = inBB[index >> WORDPOW] & 
    ~(1U << (index & (WORDLEN-1)));

  return 0;
}

int equalsBB(word aBB[], word bBB[]){
  int r = 1;
  for(size_t i = 0; i < BBLEN; i++)
    r = r && (aBB[i] == bBB[i]);

  return r;
}

int issetBB(word BB[], size_t index){
  assert(index < LENGTH);
  return BB[index >> WORDPOW] & (1U << (index & (WORDLEN-1)));
}

int fixlastwordBB(word inBB[], word outBB[]){
  outBB[BBLEN-1] = inBB[BBLEN-1] & ((1U << (LENGTH & (WORDLEN-1)))-1);
  return 0;
}

int init_dirBBmask(void){
  /* first naive implementation */
  for(size_t i = 0; i<LENGTH; i++){
    for(size_t j = D_LEFT; j < D_COUNT; j++){
      size_t col, row;
      row = i/WIDTH;
      col = i%WIDTH;
      switch (j) {
        case D_LEFT :
          if (col)
            setBB(arr_dirBBmask[j], arr_dirBBmask[j], i);
          break;

        case D_RIGHT:
          if (col != WIDTH-1) 
            setBB(arr_dirBBmask[j], arr_dirBBmask[j], i);
          break;

        case D_UPLEFT :
          if ((!(row&1)) && (col)) 
            setBB(arr_dirBBmask[j], arr_dirBBmask[j], i);
          break;

        case D_UP :
          setBB(arr_dirBBmask[j], arr_dirBBmask[j], i);
          break;

        case D_UPRIGHT :
          if (((row&1)) && (col!=WIDTH-1)) 
            setBB(arr_dirBBmask[j], arr_dirBBmask[j], i);
          break;

        case D_DOWNLEFT :
          if((!(row&1)) && (col))
            setBB(arr_dirBBmask[j], arr_dirBBmask[j], i);
          break;

        case D_DOWN :
          setBB(arr_dirBBmask[j], arr_dirBBmask[j], i);
          break;

        case D_DOWNRIGHT :
          if(((row&1)) && (col!=WIDTH-1)) 
            setBB(arr_dirBBmask[j], arr_dirBBmask[j], i); 
          break;

        default:
          break;
      }
    }
  }

  return 0;
}

/* require init... to be call b4*/
int moveBBdir(word inBB[], word outBB[], size_t d){
  assert(d < D_COUNT);
  word tmpBB[BBLEN] = {0};
  interBB(inBB, arr_dirBBmask[d], tmpBB);
  shiftBB(tmpBB, outBB, arr_dir[d]);
  return 0;
}

int containingBB(word inBB[], word outBB[], size_t thickness){
  copyBB(outBB, inBB);
  for(size_t i = 0; i<thickness; i++){
    word accBB[BBLEN] = {0};
    word tmpBB[BBLEN] = {0};
    for(size_t j = D_LEFT; j<D_COUNT; j++){
      moveBBdir(outBB, tmpBB, j);
      unionBB(accBB, tmpBB, accBB);
    }
    unionBB(accBB, outBB, outBB);
    printBB(outBB);
  }

  return 0;
}

void printBB(word BB[]){
  for(size_t i = 0; i < HEIGHT; i++){
    for(size_t j = 0; j < WIDTH; j++){
      printf("%d ", issetBB(BB, i*WIDTH + j) && 1);
    }
    printf(i&1 ? "\n" : "\n ");
  }

  putc('\n',stdout);
}

void printhexBB(word BB[]){
  for(size_t i = 0; i < BBLEN; i++)
    printf("0x%8.8x\n", BB[i]);
  putc('\n',stdout);
}
