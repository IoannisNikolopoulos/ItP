#include <assert.h>

#include "solver.h"
#include "structs.h"

/* Word List */

void WordList_Print(WordList *word_list)
{
    for (int i = 0; i < word_list->num_words; i++)
    {
        printf("%s \n", word_list->words[i]);
    }
}

int WordList_Init(WordList *word_list, FILE* file) 
{

    char** words = NULL;
    int num_words = 0;
    char word[MAX_LINE_LENGTH];
    
    while (fscanf(file, "%s", word) == 1)
    {
        /* Resize the array of words and allocate space for the new word */
        words = realloc(words, (num_words + 1) * sizeof(char*));
        words[num_words] = malloc((strlen(word) + 1) * sizeof(char));
        /* Copy the word into the array. */
        strcpy(words[num_words], word);
        num_words++;
    }

    word_list->words = words;
    word_list->num_words = num_words;

    return num_words;
}

/* Crossword */

void Crossword_Init(Crossword *crossword, char* crossword_filename)
{
    int crossword_size, row, column;
    int i;
    FILE* crossword_file = fopen(crossword_filename, "r");
    
    fscanf(crossword_file, "%d", &crossword_size);

    crossword->size = crossword_size;
    crossword->board = (char*)malloc(crossword_size * crossword_size * sizeof(char));

    memset(crossword->board, '.', crossword_size * crossword_size * sizeof(char));

    while(fscanf(crossword_file, "%d %d", &row, &column) == 2)
    {
        row = row - 1;       // y
        column = column - 1; // x

        crossword->board[row * crossword_size + column] = '#';
    }

}
/* This function simply prints the crossword to the screen. */
void Crossword_Print(Crossword* crossword, MovesStack* moves, WordList *word_list)
{
    int i, j;

    // fill a board with words from moves
    char *board = (char*)malloc(crossword->size * crossword->size * sizeof(char));
    for (i = 0; i < crossword->size * crossword->size; i++)
    {
        board[i] = crossword->board[i];
    }
    
    for (i = 0; i <= moves->top; i++)
    {
        int x = moves->moves[i].x;
        int y = moves->moves[i].y;
        int word_index = moves->moves[i].word_index;
        int word_length = strlen(word_list->words[word_index]);

        if (moves->moves[i].direction == HORIZONTAL)
        {
            for (j = 0; j < word_length; j++)
            {
                board[INDEX(x + j, y)] = word_list->words[word_index][j];
            }
        }
        else
        {
            for (j = 0; j < word_length; j++)
            {
                board[INDEX(x, y + j)] = word_list->words[word_index][j];
            }
        }
    }

    // print the board
    for (i = 0; i < crossword->size; i++)
    {
        for (j = 0; j < crossword->size; j++)
        {
            printf("%c ", board[INDEX(j, i)]);
        }
        printf("\n");
    }

    free(board);
}

/* This function checks if the crossword is valid. Basically, it feels the 
   crossword with words from already commited moves (all moves except from the last one in the MoveStack are guaranteed to be valid).
   Then, it tries to place the last word in the move stack. */
int Crossword_IsValid(Crossword *crossword, MovesStack *moves, WordList *word_list)
{
    int x, y;
    int word_index;
    int word_length;
    int i;

    char *board = malloc(crossword->size * crossword->size * sizeof(char));
    memcpy(board, crossword->board, crossword->size * crossword->size * sizeof(char));

    for (i = 0; i < moves->top; i++)
    {
        Move move = moves->moves[i];
        word_index = move.word_index;
        word_length = strlen(word_list->words[word_index]);

        if (move.direction == HORIZONTAL)
        {
            for (x = 0; x < word_length; x++)
            {
                board[INDEX(move.x + x, move.y)] = word_list->words[word_index][x];
            }
        }
        else if (move.direction == VERTICAL)
        {
            for (y = 0; y < word_length; y++)
            {
                board[INDEX(move.x, move.y + y)] = word_list->words[word_index][y];
            }
        }
    }

    Move move = moves->moves[moves->top];
    word_index = move.word_index;
    word_length = strlen(word_list->words[word_index]);

    if (move.direction == HORIZONTAL)
    {
        for (x = 0; x < word_length; x++)
        {
            if (board[INDEX(move.x + x, move.y)] != '.' && board[INDEX(move.x + x, move.y)] != word_list->words[word_index][x])
            {
                free(board);
                return 0;
            }
        }
    }
    else if (move.direction == VERTICAL)
    {
        for (y = 0; y < word_length; y++)
        {
            if (board[INDEX(move.x, move.y + y)] != '.' && board[INDEX(move.x, move.y + y)] != word_list->words[word_index][y])
            {
                free(board);
                return 0;
            }
        }
    }

    free(board);

    return 1;
}




/* Word Lookup */

/* The word lookup struct is simply an array of arrays where each array 
   contains the indices of all the words with the given length. 
   This struct can be thought of as a simple hash table. */
void WordLookup_Init(WordLookup *word_lookup, WordList *word_list)
{
    int max_length = 0;
    for (int i = 0; i < word_list->num_words; i++)
    {
        if (strlen(word_list->words[i]) > max_length)
        {
            max_length = strlen(word_list->words[i]);
        }
    }

    word_lookup->words_of_length = (int**)malloc((max_length + 1) * sizeof(int *));
    word_lookup->num_words_of_length = (int*)malloc((max_length + 1) * sizeof(int));

    for (int i = 0; i < word_list->num_words; i++)
    {
        int length = strlen(word_list->words[i]);
        word_lookup->num_words_of_length[length]++;
    }

    for (int i = 0; i <= max_length; i++)
    {
        word_lookup->words_of_length[i] = (int*)malloc(word_lookup->num_words_of_length[i] * sizeof(int));
    }

    for (int i = 0; i < word_list->num_words; i++)
    {
        int length = strlen(word_list->words[i]);
        word_lookup->words_of_length[length][--word_lookup->num_words_of_length[length]] = i;
    }


    for (int i = 0; i < word_list->num_words; i++)
    {
        int length = strlen(word_list->words[i]);
        word_lookup->num_words_of_length[length]++;
    }

    word_lookup->max_length = max_length;

}

void WordLookup_Print(WordLookup *word_lookup, WordList *word_list)
{
    for (int i = 0; i <= word_lookup->max_length; i++)
    {
        DEBUG("Words of length %d: ", i);
        for (int j = 0; j < word_lookup->num_words_of_length[i]; j++)
        {
            DEBUG("%s ", word_list->words[word_lookup->words_of_length[i][j]]);
        }
        DEBUG("\n");
    }
}

int WordLookup_Get(WordLookup *word_lookup, WordList* word_list, char *word)
{
    int word_length = strlen(word);

    for (int i = 0; i < word_lookup->num_words_of_length[word_length]; i++)
    {
        int word_index = word_lookup->words_of_length[word_length][i];
        if (strcmp(word_list->words[word_index], word) == 0)
        {
            return word_index;
        }
    }

    return -1; /* Word not found */
}


/* Move */

void Move_Init(Move* move, int x, int y, enum Direction direction, int word_index)
{
    move->x = x;
    move->y = y;
    move->direction = direction;
    move->word_index = word_index;
}


/* Moves Stack */

void Moves_Init(MovesStack *stack, int size)
{
    stack->top = -1;
    stack->size = size;
    stack->moves = malloc(size * sizeof(Move));
}

void Moves_Push(MovesStack *stack, Move move)
{
    stack->moves[++stack->top] = move;
    assert(stack->top < stack->size);
}

Move Moves_Pop(MovesStack *stack)
{
    assert(stack->top >= 0);
    return stack->moves[stack->top--];
}

void Moves_Free(MovesStack *moves)
{
    free(moves->moves);
    moves->moves = NULL;
    DEBUG("Freed moves\n");
}

MovesStack Moves_Copy(MovesStack *stack)
{
    MovesStack new_stack;
    new_stack.top = stack->top;
    new_stack.size = stack->size;
    new_stack.moves = malloc(stack->size * sizeof(Move));
    memcpy(new_stack.moves, stack->moves, stack->size * sizeof(Move));

    return new_stack;
}

void Moves_PrintOrdered(MovesStack *moves, Crossword* crossword, WordList *word_list)
{
    int i;
    int x, y;

    for (y = 0; y < crossword->size; y++)
    for (x = 0; x < crossword->size; x++)
    {
        for (i = 0; i <= moves->top; i++)
        {
            Move move = moves->moves[i];
            if (move.direction == HORIZONTAL && move.x == x && move.y == y)
            {
                printf("%s\n", word_list->words[move.word_index]);
            }
        }
    }
    
    for (x = 0; x < crossword->size; x++)
    for (y = 0; y < crossword->size; y++)
    {

        for (i = 0; i <= moves->top; i++)
        {
            Move move = moves->moves[i];
            if (move.direction == VERTICAL && move.x == x && move.y == y)
            {
                printf("%s\n", word_list->words[move.word_index]);
            }
        }
    }
}


void Moves_Print(MovesStack *moves, WordList *word_list)
{
    int i;
    for (i = 0; i <= moves->top; i++)
    {
        Move move = moves->moves[i];
        printf("%d %d %s %d", move.x, move.y, word_list->words[move.word_index], move.word_index);
        if (move.direction == HORIZONTAL)
        {
            printf(" HORIZONTAL");
        }
        else if (move.direction == VERTICAL)
        {
            printf(" VERTICAL");
        }
        printf("\n");
    }
}
