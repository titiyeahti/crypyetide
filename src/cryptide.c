#include "cryptide.h"

/* CONSTANTS */
#define DEF(E,C,S) C
const char arr_bichar[] = {
#include "biomes.def"
};

const char arr_terchar[] = {
#include "territories.def"
};

const char arr_ctchar[] = {
#include "constructs.def"
};

const char arr_colchar[] = {
#include "colors.def"
};

#undef DEF
#define DEF(E,C,S) S
const char* const arr_biname[] = {
#include "biomes.def"
};

const char* const arr_tername[] = {
#include "territories.def"
};

const char* const arr_ctname[] = {
#include "constructs.def"
};

const char* const arr_colname[] = {
#include "colors.def"
};
#undef DEF

board_s* new_board(void){
  board_s temp = {0};
  board_s* res = malloc(sizeof(board_s));
  *res = temp;
  return res;
}

int reset_board(board_s* board){
  for(size_t i=0; i<BI_COUNT; i++)
    flushBB(board->biomes[i]);

  for(size_t i=0; i<TE_COUNT; i++)
    flushBB(board->territories[i]);

  for(size_t i=0; i<CT_COUNT; i++)
    flushBB(board->constructs[i]);

  for(size_t i=0; i<CO_COUNT; i++)
    flushBB(board->colors[i]);

  return 0;
}

void free_board(board_s* b){
  free(b);
  b = NULL;
}

void print_board(board_s* b){
  printf("Biomes\n------\n");
  for(size_t i = 0; i < HEIGHT; i++){
    for(size_t j = 0; j < WIDTH; j++){
      char c = 0;
      for(size_t bi = 0; bi < BI_COUNT; bi++){
        if(issetBB(b->biomes[bi], i*WIDTH + j)){
          c = arr_bichar[bi];
          break;
        }
      }

      printf("%c ", c);
    }

    printf(i&1 ? "\n" : "\n ");
  }
  
  printf("\nTerritories\n------------\n");

  for(size_t i = 0; i < HEIGHT; i++){
    for(size_t j = 0; j < WIDTH; j++){
      char c = ' ';
      for(size_t te = 0; te < TE_COUNT; te++){
        if(issetBB(b->territories[te], i*WIDTH + j)){
          c = arr_terchar[te];
          break;
        }
      }

      printf("%c ", c);
    }

    printf(i&1 ? "\n" : "\n ");
  }

  printf("\nConstructs\n----------\n");
  
  for(size_t i = 0; i < HEIGHT; i++){
    for(size_t j = 0; j < WIDTH; j++){
      char c = ' ';
      char d = ' ';
      for(size_t ct = 0; ct < CT_COUNT; ct++){
        if(issetBB(b->constructs[ct], i*WIDTH + j)){
          c = arr_ctchar[ct];
          for(size_t co = 0; co < CO_COUNT; co++){
            if(issetBB(b->colors[co], i*WIDTH + j)){
              d = arr_colchar[co];
              break;
            }
          }
          break;
        }
      }

      printf("%c%c", c, d);
    }

    printf(i&1 ? "\n" : "\n ");
  }
  putc('\n', stdout);
}

size_t random_draw(size_t repartition[], size_t rep_len, size_t total){
  size_t rd = rand() % total;
  size_t ecc = 0;
  for(size_t i = 0; i < rep_len; i++){
    ecc += repartition[i];
    if(rd < ecc){
      return i;
    }
  }

  perror("random_draw");
  exit(EXIT_FAILURE);
}

int random_board(board_s* board){
  size_t tiles_biomes[BI_COUNT] = {0};
  
  for(size_t i = 0; i < BI_COUNT; i++)
    tiles_biomes[i] = LENGTH/BI_COUNT;
  for(size_t i = 0; i < LENGTH % BI_COUNT; i++)
    tiles_biomes[i] ++;

  size_t tiles_territories[TE_COUNT + 1] = {0};
  for(size_t i = 0; i < TE_COUNT; i++)
    tiles_territories[i] = TILES_PER_ANNIMAL;

  tiles_territories[TE_COUNT] = LENGTH - TE_COUNT*TILES_PER_ANNIMAL;

  size_t tiles_constructs[CT_COUNT + 1] = {0};
  for(size_t i = 0; i < CT_COUNT; i++)
    tiles_constructs[i] = CO_COUNT;
  tiles_constructs[CT_COUNT] = LENGTH - CO_COUNT*CT_COUNT;


  /* rt stands for rmaining tiles */
  for(size_t rt = LENGTH; rt; rt--){
    size_t i = 0;
    word* curBB;

    /* Biomes */
    i = random_draw(tiles_biomes, BI_COUNT, rt);
    tiles_biomes[i] --;
    curBB = board->biomes[i];
    setBB(curBB, curBB, rt-1);

    /* Territories */
    i = random_draw(tiles_territories, TE_COUNT+1, rt);
    tiles_territories[i] --;
    if(i<TE_COUNT){
      curBB = board->territories[i];
      setBB(curBB, curBB, rt-1);
    }
    
    /* Constructs and colors */
    i = random_draw(tiles_constructs, CT_COUNT+1, rt);
    tiles_constructs[i] --;
    if(i<CT_COUNT){
      curBB = board->constructs[i];
      setBB(curBB, curBB, rt-1);
      curBB = board->colors[tiles_constructs[i]];
      setBB(curBB, curBB, rt-1);
    }
  }

  return 0;
}
