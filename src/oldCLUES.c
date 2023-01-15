#include "cryptide.h"
#define SAMPLESIZE 10
/*OLD*/

int main(){
  init_dirBBmask();

  size_t fails_count[3] = {0};
  double mean_number[3] = {.0F, .0F, .0F};

  for(size_t i = 0; i < SAMPLESIZE; i++){
    board_s* board = new_board();
    random_board(board);
    clues_s* clues = new_clues(board);

    for(size_t nb=3; nb<6; nb++){
      size_t count = 0;
      size_t curr[nb];
      word andBB[BBLEN] = {0};
      clues_dfs(clues, NULL, nb, pop_count32, nb, curr, andBB, &count);
      if(find_clues_dfs(clues, NULL, nb, pop_count32, curr)){
        printf("success\n\n");
      }
      if(!count) 
        fails_count[nb-3] += 1;
      else 
        mean_number[nb-3] += count / (double) SAMPLESIZE;
    }

    free(board);
    free(clues);
  }

  printf("RESULTS FOR %lu RANDOM BOARDS:\n", SAMPLESIZE);
  for(size_t i = 3; i < 6; i++)
    printf("%lu PLAYERS : mean = %lf, nb_fails = %lu\n", 
        i, mean_number[i-3], fails_count[i-3]);

  return EXIT_SUCCESS;
}
