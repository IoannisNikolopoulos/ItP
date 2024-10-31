#ifndef SOLVER_H
#define SOLVER_H

#include "structs.h"

void Crossword_Init(Crossword *crossword, char* crossword_filename);
void Crossword_Print(Crossword* crossword, MovesStack* moves, WordList *word_list);
int Crossword_IsValid(Crossword *crossword, MovesStack *moves, WordList *word_list);
void Crossword_Check(Crossword* crossword, MovesStack* moves, WordList *word_list, WordLookup *word_lookup);
int Crossword_CanWordBePlaced(Crossword *crossword, MovesStack *moves, WordList *word_list, int x, int y, enum Direction direction);

void Crossword_Solve(Crossword *crossword, WordList *word_list, MovesStack *moves, int x_start, int y_start, int* crossword_solved_flag);
void Crossword_Check(Crossword* crossword, MovesStack* moves, WordList *word_list, WordLookup *word_lookup);
int Crossword_IsSolved(Crossword *crossword, MovesStack *moves, WordList *word_list);
int Crossword_CanWordBePlaced(Crossword *crossword, MovesStack *moves, WordList *word_list, int x, int y, enum Direction direction);
int Crossword_HasMoveBeenMade(Crossword *crossword, MovesStack *moves, WordList *word_list, int x, int y, enum Direction direction);

#endif
