#include "cryptide.h"

int main(int argc, char* argv[]){
  if(argc != 2)
    exit(EXIT_FAILURE);

  size_t nb = strtoul(argv[1], NULL, 10);
  if(nb<3 || nb>5)
    exit(EXIT_FAILURE);

  init_dirBBmask();

  game_s* game = new_game(nb, 1);

  print_board(game->board);
  word tmpBB[BBLEN] = {0};
  // create a macro for god sake
  copyBB(tmpBB, game->clues->two_biomes[game->players[0].clue_id]);
  for(size_t i = 0; i < game->nb_players; i++){
    printf("player %lu, indice %lu :\n", i, game->players[i].clue_id);
    printBB(game->clues->two_biomes[game->players[i].clue_id]);
    interBB(tmpBB, game->clues->two_biomes[game->players[i].clue_id], tmpBB);
  }

  printBB(tmpBB);

  free_game(game);

  return 0;
}
