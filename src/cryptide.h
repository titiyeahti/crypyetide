#ifndef CRYPTIDE_H
#define CRYPTIDE_H
#include "bitboard.h"

/* TODO add the number of tiles in the .def for biomes and 
 * territories maybe */

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

#define TILES_PER_ANNIMAL 8

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
int reset_board(board_s* board);
void free_board(board_s* board);
void print_board(board_s* board);
int random_board(board_s* board);

/* CLUES */

#define TWO_BIOMES_CLUES_COUNT ((BI_COUNT*(BI_COUNT-1))>>1)  

/* +1 because of the territorry case */
#define ONE_TILE_MAX_CLUES_COUNT (BI_COUNT+1)

#define TWO_TILES_MAX_CLUES_COUNT (CT_COUNT + TE_COUNT)

#define THREE_TILES_MAX_CLUES_COUNT (CO_COUNT)

#define TOTAL_CLUES_COUNT (TWO_BIOMES_CLUES_COUNT \
  + ONE_TILE_MAX_CLUES_COUNT \
  + TWO_TILES_MAX_CLUES_COUNT \
  + THREE_TILES_MAX_CLUES_COUNT)

typedef struct clues clues_s;
struct clues{
  word two_biomes[TWO_BIOMES_CLUES_COUNT][BBLEN];
  word one_tile_max[ONE_TILE_MAX_CLUES_COUNT][BBLEN];
  word two_tiles_max[TWO_TILES_MAX_CLUES_COUNT][BBLEN];
  word three_tiles_max[THREE_TILES_MAX_CLUES_COUNT][BBLEN];
};

clues_s* new_clues(board_s* board, int inv);
void free_clues(clues_s* clues);
void print_clues(size_t len, clues_s clues[len]);
clues_s* copy_clues(size_t len, clues_s clues[len]);
void inverted_clues(clues_s* src, clues_s* dest);
void clues_dfs(size_t len, clues_s clues[len], size_t* ids, size_t nb, 
    pop_count_f pc,size_t depth, size_t curr[nb], 
    word andBB[], size_t* count);

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
	board_s* board;
};

/* return the index of wining player or -1 if game break*/
int run(game_s* g);

/* multi
 * display
 */

#endif
