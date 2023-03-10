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
  reset_board(board);
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

clues_s* new_clues(board_s* board, int inv){
  word tmpBB[BBLEN] = {0};
  clues_s tmp = {0};
  clues_s* ret = malloc(sizeof(clues_s)*(inv ? 2 : 1));
  *ret = tmp;

  /* DOUBLE BIOMES */
  size_t count = 0;
  for(size_t i = 0; i < BI_COUNT - 1; i++){
    for(size_t j = i + 1; j < BI_COUNT; j++){
      unionBB(board->biomes[i], board->biomes[j], ret->two_biomes[count]);
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

  if(inv)
    inverted_clues(&(ret[0]), &(ret[1]));

  return ret;
}

void free_clues(clues_s* clues){
  free(clues);
  clues = NULL;
}

void print_clues(size_t len, clues_s clues[len]){
  for(size_t i = 0; i < TOTAL_CLUES_COUNT*len; i++)
    printBB(clues->two_biomes[i]);
}

clues_s* copy_clues(size_t len, clues_s clues[len]){
  clues_s* ret = malloc(sizeof(clues_s)*len);
  memcpy(ret, clues, sizeof(clues_s)*len);
  return ret;
}

void inverted_clues(clues_s* src, clues_s* dest){
  for(size_t i = 0; i < TOTAL_CLUES_COUNT; i++)
    negBB(src->two_biomes[i], dest->two_biomes[i]);
}



/* TODO : better handling of ids -> DONE */
/* TODO debug */
int check_clues(size_t len, clues_s clues[len], size_t* ids,
    pop_count_f pc, size_t nb, size_t curr[nb]){
  word bottomBB[nb-1][BBLEN];
  word topBB[nb-1][BBLEN];
  size_t fixed_curr[nb];

  for(size_t i = 0; i < nb; i++)
    fixed_curr[i] = ids ? ids[curr[i]] : curr[i];

  copyBB(bottomBB[0], clues->two_biomes[fixed_curr[0]]);
  copyBB(topBB[0], clues->two_biomes[fixed_curr[nb-1]]);

  for(size_t i = 1; i<nb-1; i++){
    interBB(bottomBB[i-1], clues->two_biomes[fixed_curr[i]], bottomBB[i]);
    interBB(topBB[i-1], clues->two_biomes[fixed_curr[nb-i-1]], topBB[i]);
  }
  
  if(popcountBB(bottomBB[nb-2], pc) < 2 || 
      popcountBB(topBB[nb-2], pc) < 2)
    return 0;

  for(size_t k = 1; k<nb-1; k++){
    word tmpBB[BBLEN] = {0};
    interBB(topBB[nb-k-2], bottomBB[k-1], tmpBB);
    size_t card = popcountBB(tmpBB, pc);
    if(card < 2)
      return 0;
  }

  return 1;
}

int find_clues(size_t len, clues_s clues[len], size_t* ids, size_t nb, 
    pop_count_f pc, size_t depth, size_t curr[nb], word andBB[]){
  if(depth > 0){
    for(size_t i = depth == nb ? 0 : curr[nb-1-depth] + 1; 
        i<TOTAL_CLUES_COUNT*len; i++){
      /* 16 bytes memory per node warning */
      size_t id = ids ? ids[i] : i;
      word tmpBB[BBLEN] = {0};

      if(depth == nb)
        copyBB(tmpBB, clues->two_biomes[id]);
      else
        interBB(andBB, clues->two_biomes[id], tmpBB);

      size_t nb_elt = popcountBB(tmpBB, pc);

      /* rec call */
      if(depth > 1 && nb_elt > 1){
        curr[nb-depth] = i;
        if(find_clues(len, clues, ids, nb, pc, depth - 1, curr, tmpBB))
          return 1;
      }
      if(depth == 1 && nb_elt == 1){
        curr[nb-depth] = i;
        return check_clues(len, clues, ids, pc, nb, curr);
      }
    }
    return 0;
  }
  return 0;
}


void clues_dfs(size_t len, clues_s clues[len], size_t* ids, size_t nb, 
    pop_count_f pc,size_t depth, size_t curr[nb], 
    word andBB[], size_t* count){
  if(depth > 0){
    for(size_t i = depth == nb ? 0 : curr[nb-1-depth] + 1; 
        i<TOTAL_CLUES_COUNT*len; i++){
      /* 16 bytes memory per node warning */
      size_t id = ids ? ids[i] : i;
      word tmpBB[BBLEN] = {0};

      if(depth == nb)
        copyBB(tmpBB, clues->two_biomes[id]);
      else
        interBB(andBB, clues->two_biomes[id], tmpBB);

      size_t nb_elt = popcountBB(tmpBB, pc);

      /* rec call */
      if(depth > 1 && nb_elt > 1){
        curr[nb-depth] = i;
        clues_dfs(len, clues, ids, nb, pc, depth - 1, curr, tmpBB, count);
      }
      if(depth == 1 && nb_elt == 1){
        curr[nb-depth] = i;
        /*
        printf("possible clues subset :"); 
        for(size_t j = 0; j < nb; j++)
          printf("(%lu) ", ids ? ids[curr[j]] : curr[j]);

        printf("\n");
        */
        *count += check_clues(len, clues, ids, pc, nb, curr);
      }
    }
  }
}

game_s* new_game(size_t nb, int inv){
  game_s* ret = malloc(sizeof(game_s));
  ret->nb_players = nb;
  ret->clues_len = (size_t) inv + 1;
  ret->board = new_board();
  ret->players = malloc(sizeof(player_s)*ret->nb_players);
  
  for(size_t i = 0; i < ret->nb_players; i++){
    flushBB(ret->players[i].answers[0]);
    flushBB(ret->players[i].answers[1]);
  }

  size_t ids_len = TOTAL_CLUES_COUNT*ret->clues_len;
  size_t ids[ids_len];
  
  // init
  for(size_t i = 0; i < ids_len; i++){
    ids[i] = i;
  }

  // shuffle
  for(size_t i = 0; i < ids_len-1; i++){
    // random draw between current id and the end 
    size_t rd = (rand() % (ids_len-i)) + i;
    size_t tmp = ids[i];
    ids[i] = ids[rd];
    ids[rd] = tmp;
  }

  size_t curr[nb];
  for(;;){
    random_board(ret->board);
    ret->clues = new_clues(ret->board, inv);
    if(find_clues(ret->clues_len, ret->clues, ids, ret->nb_players, 
          pop_count32, ret->nb_players, curr, NULL)){
      
      for(size_t i = 0; i < ret->nb_players; i++)
        ret->players[i].clue_id = ids[curr[i]];

      break;
    }

    free_clues(ret->clues);
  }

  return ret;
}

void free_game(game_s* game){
  free(game->players);
  game->players = NULL;
  free_board(game->board);
  free_clues(game->clues);
  free(game);
}

int ask_player(game_s* game, size_t player_id, size_t tile){
  if(tile >= LENGTH)
    return -1;

  word tmpBB[BBLEN] = {0};

  /* checks if someone asked for this tile
   * is it necessary here ? */
  for(size_t i = 0; i < game->nb_players; i++){
    unionBB(game->players[i].answers[0], tmpBB, tmpBB);
    unionBB(game->players[i].answers[1], tmpBB, tmpBB);
  }

  if(issetBB(tmpBB, tile))
    return -1;

  /* totally not obufcated line*/
  return issetBB(game->clues->two_biomes[game->players[player_id].clue_id], 
      tile);
}


