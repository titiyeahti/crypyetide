#include "cryptide.h"
#include <time.h>

int main(int argc, char* argv[]){
  if(argc != 2)
    printf("usage : %s number\n", argv[0]);
  size_t sample_size = strtoul(argv[1], NULL, 10);
  clock_t c;
  clock_t cmax[2][3] = {0};
  double cmean[2][3] = {.0};
  init_dirBBmask();
  board_s* board = new_board();

  for(size_t i = 0; i < sample_size; i++){
    for(int inv = 0; inv < 2; inv ++){
      for(size_t nb = 3; nb < 6; nb ++){
        c = clock();
        size_t curr[nb];
        for(;;){
          reset_board(board);
          random_board(board);
          clues_s* clues = new_clues(board, inv);
          
          if(find_clues((size_t)inv + 1, clues, NULL, nb, 
              pop_count32, nb, curr, NULL)){
            free_clues(clues);
            break;
          }

          free_clues(clues);
        }
        c = clock() - c;
        if(c > cmax[inv][nb-3])
          cmax[inv][nb-3] = c;

        cmean[inv][nb-3] += c/sample_size;
      }
    }
  }

  for(int inv = 0; inv <2; inv ++){
    for(size_t nb = 3; nb < 6; nb++){
      printf("%lu PLAYERS, HARDMODE = %d, max = %lf, mean = %lf\n",
          nb, inv, (double) cmax[inv][nb-3]/CLOCKS_PER_SEC, 
          cmean[inv][nb-3]/CLOCKS_PER_SEC);
    }
  }
  return 0;
}
