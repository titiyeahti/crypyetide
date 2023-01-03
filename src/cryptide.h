#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "bitboard.h"

enum biomes{
  desert,
  swamp,
  sea,
  forest,
  mountain,
  nb_biomes
};

enum color{
  black,
  white,
  green,
  blue,
  nb_color
};

enum structure{
  hut,
  stone,
  nb_struture
};

enum territory{
  cheetah,
  bear,
  nb_territory
};


typedef struct slot slot;

struct slot {
  uint8_t neigh[6];
  uint32_t stuff;
};

int neigh_index(size_t index, uint8_t neigh[6]);

int init_neigh(slot board[HEIGHT*WIDTH]);
