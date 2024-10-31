#include "solver.h"
#include "structs.h"

#include <unistd.h>

extern WordLookup word_lookup;

void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* This function returns the number of words that can be placed horizontally or vertically in the crossword puzzle. */
void Crossword_CountWordsToFill(Crossword* crossword, MovesStack* moves, WordList* word_list, int* count_horizontal_out, int* count_vertical_out) 
{
    int count_horizontal = 0, count_vertical = 0;
    int x, y;

    for (x = 0; x < crossword->size; x++)
    for (y = 0; y < crossword->size; y++)
    {
        count_horizontal += Crossword_CanWordBePlaced(crossword, moves, word_list, x, y, HORIZONTAL);
        count_vertical += Crossword_CanWordBePlaced(crossword, moves, word_list, x, y, VERTICAL);
    }

*count_horizontal_out = count_horizontal;
*count_vertical_out = count_vertical;
}

/* This function checks if the given word can be placed in the crossword puzzle with regard to the words already placed. */
void Crossword_CheckMoves(Crossword *crossword, MovesStack *moves, WordList *word_list, WordLookup *word_lookup, char* word, enum Direction direction)
{
    int x, y;
    int word_index = WordLookup_Get(word_lookup, word_list, word);

    if (word_index == -1)
    {
        printf("Word \"%s\" not in dictionary\n", word);
        exit(EXIT_FAILURE);
    }

    for (y = 0; y < crossword->size; y++)
    for (x = 0; x < crossword->size; x++)
    {
        if (direction == VERTICAL) Swap(&x, &y);
    
        if (!Crossword_CanWordBePlaced(crossword, moves, word_list, x, y, direction))
        {
            if (direction == VERTICAL) Swap(&x, &y); /* Need to swap the indexes back for the next iteration */
            continue;
        }

        Move move;
        Move_Init(&move, x, y, direction, word_index);

        Moves_Push(moves, move);

        if (!Crossword_IsValid(crossword, moves, word_list))
        {
            Moves_Pop(moves);
            printf("Word \"%s\" cannot be placed\n", word);
            exit(EXIT_FAILURE);
        }

        return;
    }
}

/* This function tries to place all the words entered from stdin in the crossword puzzle. 
   It first checks if the number of words entered is correct and then tries to place them. 
   The words are saved in the WordList words_to_place. */
void Crossword_Check(Crossword* crossword, MovesStack* moves, WordList *word_list, WordLookup *word_lookup)
{
    int i;
    int crossword_valid = 0;

    int n_vertical, n_horizontal;
    WordList words_to_place;

    Crossword_CountWordsToFill(crossword, moves, word_list, &n_horizontal, &n_vertical);
    WordList_Init(&words_to_place, stdin);

    if (words_to_place.num_words < n_horizontal + n_vertical)
    {
        printf("Not enough words\n");
        exit(EXIT_FAILURE);
    }

    if (words_to_place.num_words > n_horizontal + n_vertical)
    {
        printf("More words than needed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n_horizontal; i++)
    {
        char *word = words_to_place.words[i];
        Crossword_CheckMoves(crossword, moves, word_list, word_lookup, word, HORIZONTAL);
    }

    for (i = 0; i < n_vertical; i++)
    {
        char *word = words_to_place.words[i + n_horizontal];
        Crossword_CheckMoves(crossword, moves, word_list, word_lookup, word, VERTICAL);
    }
}

/* This function checks if the crossword puzzle is solved. 
   It does so by checking if all the cells that can have a word placed in them have a word placed in them. 
   It is guarenteed that the crossword is already valid and all the words that are already placed are valid. */
int Crossword_IsSolved(Crossword *crossword, MovesStack *moves, WordList *word_list)
{
    int x, y;
    int i;

    for (y = 0; y < crossword->size; y++)
    for (x = 0; x < crossword->size; x++)
    {
        if (Crossword_CanWordBePlaced(crossword, moves, word_list, x, y, HORIZONTAL) && !Crossword_HasMoveBeenMade(crossword, moves, word_list, x, y, HORIZONTAL))
        {
            DEBUG("word can be placed at %d, %d horizontally, but no move has been made\n", x, y);

            return 0;
        }

        if (Crossword_CanWordBePlaced(crossword, moves, word_list, x, y, VERTICAL) && !Crossword_HasMoveBeenMade(crossword, moves, word_list, x, y, VERTICAL))
        {
            DEBUG("word can be placed at %d, %d vertically, but no move has been made\n", x, y);
            return 0;
        }

    }

return 1;

}
/* This function checks if a move has been made at a given cell and with a given direction. */
int Crossword_HasMoveBeenMade(Crossword *crossword, MovesStack *moves, WordList *word_list, int x, int y, enum Direction direction)
{
    int i;
    for (i = 0; i <= moves->top; i++)
    {
        Move move = moves->moves[i];
        if (move.x == x && move.y == y && move.direction == direction)
        {
            return 1;
        }
    }

    return 0;
}
/* This function uses the Crossword_HasMoveBeenMade function to check if a word can be finally be placed in a cell. 
    The way we think about is that we try to place words around '#' characters or at the crossword's edge. */
int Crossword_CanWordBePlaced(Crossword *crossword, MovesStack *moves, WordList *word_list, int x, int y, enum Direction direction)
{
    int word_length, word_index;
    int i;

    if (Crossword_HasMoveBeenMade(crossword, moves, word_list, x, y, direction))
    {
        return 0;
    }
    
    if (x < 0 || y < 0 || x >= crossword->size || y >= crossword->size)
    {
        return 0;
    }

    if (crossword->board[INDEX(x, y)] == '#')
    {
        return 0;
    }

    if (direction == HORIZONTAL)
    {
        if ((x > 0 && crossword->board[INDEX(x - 1, y)] == '#') || x == 0)
        {
            return 1;
        }

        if ((x < crossword->size - 1 && crossword->board[INDEX(x + 1, y)] == '#') || x == crossword->size - 1)
        {
            return 0;
        }
    }
    else if (direction == VERTICAL)
    {
        if ((y < crossword->size - 1 && crossword->board[INDEX(x, y + 1)] == '#') || y == crossword->size - 1)
        {
            return 0;
        }

        if ((y > 0 && crossword->board[INDEX(x, y - 1)] == '#') || y == 0)
        {
            return 1;
        }
    }

    return 0;
}

/* This function returns how many letters are in a word that can be placed in a cell. 
   It basically counts all the '.' characters until it reaches a '#' character or the crossword's edge. */
int Crossword_GetWordLength(Crossword *crossword, MovesStack *moves, WordList *word_list, int x, int y, enum Direction direction)
{
    int word_length, word_index;
    int i;

    if (direction == HORIZONTAL)
    {
        for (i = x; i < crossword->size; i++)
        {
            if (crossword->board[INDEX(i, y)] == '#')
            {
                return i - x;
            }
        }

        return crossword->size - x;
    }
    else if (direction == VERTICAL)
    {
        for (i = y; i < crossword->size; i++)
        {
            if (crossword->board[INDEX(x, i)] == '#')
            {
                return i - y;
            }
        }

        return crossword->size - y;
    }

    return 0;
}

int Crossword_Search(Crossword *crossword, WordList *word_list, MovesStack *moves, int x, int y, enum Direction direction, int* crossword_solved_flag)
{

    int word_length, lenth_word_index;

    if (!Crossword_CanWordBePlaced(crossword, moves, word_list, x, y, direction) || Crossword_HasMoveBeenMade(crossword, moves, word_list, x, y, direction))
    {
        DEBUG("search: can't place word at (%d, %d) in direction %d \n", x, y, direction);
        return 0;
    }

    word_length = Crossword_GetWordLength(crossword, moves, word_list, x, y, direction);

    if (word_length == 0)
    {
        return 0;
    }

    DEBUG("Trying to place a word of length %d at (%d, %d) in direction %d \n", word_length, x, y, direction);
    DEBUG("search: word_length: %d x: %d y: %d direction: %d \n", word_length, x, y, direction);

    for (lenth_word_index = 0; lenth_word_index < word_lookup.num_words_of_length[word_length]; lenth_word_index++)
    {

        int word_index = word_lookup.words_of_length[word_length][lenth_word_index];

        Move move;
        Move_Init(&move, x, y, direction, word_index);

        Moves_Push(moves, move);

        if (Crossword_IsValid(crossword, moves, word_list))
        {
            if (Crossword_IsSolved(crossword, moves, word_list))
            {
                *crossword_solved_flag = 1;
                return 1;
            }

            Crossword_Solve(crossword, word_list, moves, x, y, crossword_solved_flag);

            if (*crossword_solved_flag)
            {
                return 1;
            }

            Moves_Pop(moves);

        }
        else
        {
            DEBUG("Word %s is not valid \n", word_list->words[word_index]);
            Moves_Pop(moves);
        }

    }

    return 0;
}

/* This function is the main function that solves the crossword puzzle.
   It tries to place words horizontally and vertically.
   For each cell, it searches for a word to place when a placeable word has been
   found, it is appened to the moves stack and the function is called recursively.
   For now, there is no heuristic to choose the next word to place and simply iterates over the word list, 
   choosing words with the appropriate length.
*/
void Crossword_Solve(Crossword *crossword, WordList *word_list, MovesStack *moves, int x_start, int y_start, int* crossword_solved_flag)
{
    int x, y;
    int i;

    x_start = 0;
    y_start = 0;

    for (y = y_start; y < crossword->size; y++)
    {
        for (x = x_start; x < crossword->size; x++)
        {
            DEBUG("solve: trying to place a word at (%d, %d) \n", x, y);

            // Horizontal placement
            if (Crossword_Search(crossword, word_list, moves, x, y, HORIZONTAL, crossword_solved_flag))
            {
                return;
            }
            else 
            {
                
            }

            // Vertical placement
            if(Crossword_Search(crossword, word_list, moves, x, y, VERTICAL, crossword_solved_flag))
            {
                return;
            }
            else 
            {
            }
        }
    }

}
