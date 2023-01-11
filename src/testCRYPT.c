#include "cryptide.h"
#include <time.h>

int main(){
  srand(time(NULL));

  board_s* board = new_board();
  for(size_t i = 0; i<100; i++){
    reset_board(board);
    random_board(board);
  }
  free_board(board);
  return EXIT_SUCCESS;
}
