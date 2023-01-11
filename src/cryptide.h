#ifndef CRYPTIDE_H
#define CRYPTIDE_H
#include "bitboard.h"

/* TODO add the number of tiles in the .def for biomes and 
 * territories */

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

/* try to proove that founding a set of clues reducing to 1 
 * tile uis sufficient for map generation */
typedef struct board board_s;
struct board{
  word biomes[BI_COUNT][BBLEN];
  word territories[TE_COUNT][BBLEN];
  word constructs[CT_COUNT][BBLEN];
  word colors[CO_COUNT][BBLEN];
};

board_s* new_board(void);
void free_board(board_s* board);
void print_board(board_s* board);
int random_board();

typedef struct clues clues_s;
struct clues{
  board_s * b;
  word doublebiome[(BI_COUNT*(BI_COUNT-1))>>1][BBLEN];
  /* +1 because of the territorry case */
  word bioOne[BI_COUNT+1][BBLEN];
  word terctTwo[CT_COUNT+TE_COUNT][BBLEN];
  word colorThree[CO_COUNT][BBLEN];
};

clues_s* new_clues(board_s* board);
void free_clues(clues_s* clues);
void print_clues(clues_s* clues);
int init_clues(clues_s* ret, board_s* board);
clues_s* copy_clues(clues_s* clues);
clues_s* inverted_clues(clues_s* clues);

#define TOTAL_CLUES_COUNT (((BI_COUNT*(BI_COUNT-1))>>1) \
  + BI_COUNT + 1 \
  + CT_COUNT + TE_COUNT \
  + CO_COUNT)

typedef struct player player_s;
struct player{
  word neg_answer[BBLEN];
  word pos_answer[BBLEN];
  /* when the clue is no longer relevant set its slot to NULL*/
  word* valid_clues[TOTAL_CLUES_COUNT];
  /* same as upper but not and array if no advanced clues */
  word** valid_inv_clues; 
  size_t nb_clues;
};

typedef struct game game_s;
struct game{
	player_s* players;
	board* board;
};

/* return the index of wining player or -1 if game break*/
int run(game_s* g);

/* multi
 * display
 */

#endif
