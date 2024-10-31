#ifndef STRUCTS_H
#define STRUCTS_H

// #define DEBUG /* use if debugging */

#ifdef DEBUG
#undef DEBUG
#define DEBUG(args...) fprintf(stdout, args)
#else
#define DEBUG(args...) /* do nothing */
#endif

#define MAX_LINE_LENGTH 64
#define INDEX(x, y) ((x) + (y) * crossword->size)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Direction {
    HORIZONTAL,
    VERTICAL
};

typedef struct {
    char **words;
    int num_words;
} WordList;

typedef struct {
    int **words_of_length;
    int *num_words_of_length;
    int max_length;
} WordLookup;

typedef struct {
    char *board;
    int size;

} Crossword;

typedef struct {
    int x, y;
    enum Direction direction; 
    int word_index;
    int tried;
} Move;

typedef struct {
    int top, size;
    Move* moves;
} MovesStack;


int WordList_Init(WordList *word_list, FILE* file);
void WordList_Print(WordList *word_list);

void WordLookup_Init(WordLookup *word_lookup, WordList *word_list);
void WordLookup_Print(WordLookup *word_lookup, WordList *word_list);
int WordLookup_Get(WordLookup *word_lookup, WordList* word_list, char *word);

void Move_Init(Move* move, int x, int y, enum Direction direction, int word_index);

void Moves_Init(MovesStack *stack, int size);
void Moves_Init_stdin(MovesStack *stack, Crossword* crossword);
Move Moves_Pop(MovesStack *stack);
void Moves_Push(MovesStack *stack, Move move);
void Moves_Free(MovesStack *moves);
void Moves_Print(MovesStack *moves, WordList *word_list);
void Moves_PrintOrdered(MovesStack *moves, Crossword* crossword, WordList *word_list);

#endif
