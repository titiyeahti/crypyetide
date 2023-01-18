#include "cryptide.h"

int main(){
  init_dirBBmask();
  board_s* board = new_board();
  random_board(board);

  clues_s* simple = new_clues(board, 0);
  clues_s* hard = new_clues(board, 1);

  for(size_t nb = 3; nb<6; nb++){
    size_t count = 0;
    size_t curr[nb];
    word tmpBB[BBLEN];
    clues_dfs(1, simple, NULL, nb, pop_count, nb, curr, tmpBB, &count);
    printf("LEN = 1, NB = %lu, COUNT = %lu\n\n", nb, count);
    count = 0;
    clues_dfs(2, hard, NULL, nb, pop_count, nb, curr, tmpBB, &count);
    printf("LEN = 2, NB = %lu, COUNT = %lu\n\n", nb, count);
  }

  free_board(board);
  free_clues(simple);
  free_clues(hard);

  return EXIT_SUCCESS;
}
