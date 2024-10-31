// Write a program that takes the coordinates of black tiles in a crossword puzzle and a list of words and then solves the crossword puzzle
// by filling in the white squares with letters from the words.
// The program should print out the crossword puzzle with the words filled in.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "structs.h"
#include "solver.h"

WordLookup word_lookup;

/* This application provides the following arguments:
 * -dict <filename> - the dictionary file to use
 * -draw - draw the crossword puzzle
 * -check - check the crossword puzzle with words entered from stdin
 * <filename> - the crossword puzzle file
*/

int main (int argc, char *argv[])
{
    char *dict_filename = NULL;
    char *crossword_filename = NULL;
    int i;
    int draw = 0;
    int check = 0;
    
    Crossword crossword;
    WordList word_list;
    MovesStack moves;

    crossword_filename = argv[1];
    if (crossword_filename == NULL)
    {
        printf("Please specify a crossword file!\n");
        exit(EXIT_FAILURE);
    }

    for(i = 1; i < argc; i++ ) 
    {
        if( strcmp( argv[i], "-dict" ) == 0 ) {
            dict_filename = argv[i+1];
        }
        if( strcmp( argv[i], "-draw" ) == 0 ) {
            draw = 1;
        }
        if( strcmp( argv[i], "-check" ) == 0 ) {
            check = 1;
        }
    }

    if (dict_filename == NULL)
    {
        dict_filename = "Words.txt";
    }

    FILE* dict_file = fopen(dict_filename, "r");

    if (dict_file == NULL)
    {
        fprintf(stderr, "Could not open dictionary file!\n");
        exit(EXIT_FAILURE);
    }

    WordList_Init(&word_list, dict_file);
    Crossword_Init(&crossword, crossword_filename);
    WordLookup_Init(&word_lookup, &word_list);
    Moves_Init(&moves, crossword.size * crossword.size);

    if (check)
    {
        Crossword_Check(&crossword, &moves, &word_list, &word_lookup);
    }

    if (!check)
    {
        int crossword_solved_flag = 0;
        Crossword_Solve(&crossword, &word_list, &moves, 0, 0, &crossword_solved_flag);
        
        if (!draw)
            Moves_PrintOrdered(&moves, &crossword, &word_list);
    }

    // wordlookup_print(&word_lookup, &word_list);
    
    if (draw)
    {
        Crossword_Print(&crossword, &moves, &word_list);
    }

    return EXIT_SUCCESS;
}
