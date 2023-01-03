#ifndef CRYPTIDE_H
#define CRYPTIDE_H
#include "bitboard.h"

/* GAME CONSTANTS */
/* BIOMES */
#define DEF(E,C,S) E
enum biomes{
#include "biomes.def"
};

extern const char* const arr_biname[];
extern const char arr_bichar[];

/* TERRITORIES */
enum territories{
#include "territories.def"
};

extern const char* const arr_tername[];
extern const char arr_terchar[];

/* CONSTRUCTS */
enum constructs{
#include "constructs.def"
};

extern const char* const arr_ctname[];
extern const char arr_ctchar[];

/* COLORS */
enum colors{
#include "colors.def"
};

extern const char* const arr_colname[];
extern const char arr_colchar[];

#undef DEF

typedef struct board board_s;
struct board{
  word biomes[BI_COUNT][BBLEN];
  word territories[TE_COUNT][BBLEN];
  word constructs[CT_COUNT][BBLEN];
  word colors[CO_COUNT][BBLEN];
}

typedef struct clues clues_s;
struct clues{
  board_s * b;
  word doublebiome[(BI_COUNT*(BI_COUNT-1))>>1][BBLEN];
  /* +1 because of the territorry case */
  word bioOne[BI_COUNT+1][BBLEN];
  word terctTwo[CT_COUNT+TE_COUNT][BBLEN];
  word colorThree[CO_COUNT][BBLEN];
}



#endif
