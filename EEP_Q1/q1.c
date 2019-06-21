/**
 *    Copyright (c) 2018 by Hyoung Bok Min, All rights reserved.
 *
 *     =============================================================
 *     You must use function "mymalloc()" for memory allocation, and
 *     you have to use function "myfree()" for disposing memory.
 *     DO NOT USE "malloc()/free()" FOR DYNAMIC MEMORY ALLOCATION.
 *     =============================================================
 *
 *    Quiz #1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "memcheck.h"

typedef enum {RES, CAP, BAT, IND} COMP_TYPE;

/**
 *    ====================================================================
 *    YOU MUST USE THE FOLLOWING 2 FUNCTIONS FOR DYNAMIC MEMORY ALLOCATION.
 *
 * void *mymalloc(int);
 * void myfree(void *);
 *    ====================================================================*/


char **data_file(char *infile, char *outfile,
                    COMP_TYPE **comptype_array, int *num_comps);
int  count_num_comps(COMP_TYPE *comptype_array, int num_comps,
                    int *numr, int *numc, int *numl);
void list_name(COMP_TYPE *comptype_array, char **name_array, int num_comps);
void dispose_memory(char **name_array, COMP_TYPE *comptype_array,
                    int num_comps);


int main(void) {
    char *infile = "in.bin";      // input binary data file 
    char *outfile = "q1out.txt";  // output text data file
    char **name_array;            // array of component names
    COMP_TYPE *comptype_array;    // array of component types
    int  num_comps;               // number of components in file
    int  numr, numc, numl, numb;  // number of resistors, etc

    /**
     *  Read binary data file "in.bin", which contains component data,
     *  and write the data to text file.
     */
    printf("- Reading binary data file \"%s\",", infile);
    printf(" and write to text file \"%s\"\n", outfile);
    name_array = data_file(infile, outfile, &comptype_array, &num_comps);
    if (!name_array)
        return 1;

    /**
     *  Count the number of components.
     */
    printf("- Counting component types in the circuit\n");
    numb = count_num_comps(comptype_array, num_comps, &numr, &numc, &numl);
    printf("number of batteries   = %d\n", numb);
    printf("number of resistors   = %d\n", numr);
    printf("number of captacitors = %d\n", numc);
    printf("number of inductors   = %d\n", numl);

    /**
     *  List component names for each type
     */
    printf("- Listing component names of each type\n");
    list_name(comptype_array, name_array, num_comps);

    /**
     *  Epilog
     */
    dispose_memory(name_array, comptype_array, num_comps);

    /* DO NOT REMOVE THE FOLLOWING LINE */
    check_memory();
    return 0;
}

/*------ q1.c ------*/
