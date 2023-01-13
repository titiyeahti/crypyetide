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

clues_s* new_clues(board_s* board){
  word tmpBB[BBLEN] = {0};
  clues_s tmp = {0};
  clues_s* ret = malloc(sizeof(clues_s));
  *ret = tmp;

  /* DOUBLE BIOMES */
  size_t count = 0;
  for(size_t i = 0; i < BI_COUNT - 1; i++){
    for(size_t j = i + 1; j < BI_COUNT; j++){
      /*printf("%s(%c) OR %s(%c), count = %lu \n",arr_biname[i], arr_bichar[i],
        arr_biname[j], arr_bichar[j], count);*/

      /*
         printBB(ret->two_biomes[count]);
         */
      unionBB(board->biomes[i], board->biomes[j], ret->two_biomes[count]);
      /*printBB(ret->two_biomes[count]);

        char buff[20];
        fgets(buff, 19, stdin);
        */

      count++;
    }
  }

  /* ONE TILE MAX */
  for(size_t i = 0; i < BI_COUNT; i++)
    adjacencyBB(board->biomes[i], ret->one_tile_max[i], 1);

  for(size_t i = 0; i < TE_COUNT; i++)
    unionBB(tmpBB, board->territories[i], tmpBB);

  adjacencyBB(tmpBB, ret->one_tile_max[BI_COUNT], 1);

  /* TWO TILES MAX */
  for(size_t i = 0; i < TE_COUNT; i++)
    adjacencyBB(board->territories[i], ret->two_tiles_max[i], 2);

  for(size_t i = 0; i < CT_COUNT; i++)
    adjacencyBB(board->constructs[i], ret->two_tiles_max[i+TE_COUNT], 2);

  /* THREE TILES MAX */
  for(size_t i = 0; i < CO_COUNT; i++)
    adjacencyBB(board->colors[i], ret->three_tiles_max[i], 3);

  return ret;
}

void free_clues(clues_s* clues){
  free(clues);
  clues = NULL;
}

void print_clues(clues_s* clues){
  for(size_t i = 0; i < TOTAL_CLUES_COUNT; i++)
    printBB(clues->two_biomes[i]);
}

clues_s* copy_clues(clues_s* clues){
  clues_s* ret = malloc(sizeof(clues_s));
  memcpy(ret, clues, sizeof(clues_s));
  return ret;
}

clues_s* inverted_clues(clues_s* clues){
  clues_s* ret = copy_clues(clues);
  for(size_t i = 0; i < TOTAL_CLUES_COUNT; i++)
    negBB(clues->two_biomes[i], ret->two_biomes[i]);

  return ret;
}

void clues_dfs(clues_s* clues, size_t* ids, size_t nb, pop_count_f pc,
    size_t depth, size_t curr[nb], word andBB[], size_t* count){
  if(depth > 0){
    for(size_t i = depth == nb ? 0 : curr[nb-1-depth] + 1; 
        i<TOTAL_CLUES_COUNT; i++){
      /* 16 bytes memory per node warning */
      size_t id = ids ? ids[i] : i;
      word tmpBB[BBLEN] = {0};

      if(depth == nb)
        copyBB(tmpBB, clues->two_biomes[id]);
      else
        interBB(andBB, clues->two_biomes[id], tmpBB);

      size_t nb_elt = popcountBB(tmpBB, pc);

      /* rec call */
      if(depth > 1 && nb_elt > 2){
        curr[nb-depth] = i;
        clues_dfs(clues, ids, nb, pc, depth - 1, curr, tmpBB, count);
      }
      if(depth == 1 && nb_elt == 1){
        curr[nb-depth] = i;
        /*
        printf("possible clues subset :"); 
        for(size_t j = 0; j < nb; j++)
          printf("(%lu) ", ids ? ids[curr[j]] : curr[j]);

        printf("\n");
        */
        *count += 1;
      }
    }
  }
}
