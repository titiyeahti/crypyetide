#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "constants.def"

#define DEF(E,V,S) E
enum directions{
#include "directions.def"
};
#undef DEF

extern const int const arr_dir[];

extern const char* const arr_dirname[];

extern word arr_dirBBmask[][BBLEN];

int shiftBB(word inBB[], word outBB[], int value);
int shiftleftBB(word inBB[], word outBB[], size_t value);
int shiftrightBB(word inBB[], word outBB[], size_t value);
int unionBB(word aBB[], word bBB[], word outBB[]);
int interBB(word aBB[], word bBB[], word outBB[]);
int xorBB(word aBB[], word bBB[], word outBB[]);
int negBB(word inBB[], word outBB[]);
int copyBB(word destBB[], word srcBB[]);
int setBB(word inBB[], word outBB[], size_t index);
int unsetBB(word inBB[], word outBB[], size_t index);
int flushBB(word inBB[]);
int equalsBB(word aBB[], word bBB[]);
int iszeroBB(word inBB[]);
int issetBB(word BB[], size_t index);
int fixlastwordBB(word inBB[], word outBB[]);
int init_dirBBmask(void);
int moveBBdir(word inBB[], word outBB[], size_t d);
int adjacencyBB(word inBB[], word outBB[], size_t thickness);
void printBB(word BB[]);
void printhexBB(word BB[]);


typedef size_t pop_count_f(word);
#if(WORDLEN == 32)
size_t pop_count32(word w);
#endif
size_t pop_count(word w);

size_t popcountBB(word inBB[], pop_count_f bc);

#endif
