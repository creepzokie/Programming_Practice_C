/**
 *    Copyright (c) 2012-2018 by Hyoung Bok Min, All rights reserved.
 *    For copyright and license information, please visit
 *    http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *    =============================================================
 *     You must use function "mymalloc()" for memory allocation, and
 *     you have to use function "myfree()" for disposing memory.
 *     DO NOT USE "malloc()/free()" FOR DYNAMIC MEMORY ALLOCATION.
 *     Also, use "myfopen()/myfclose()" FOR FILE I/O.
 *
 *     NEVER MODIFY "person.h"
 *    =============================================================
 *
 *    Lab : Structures and Unions
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "person.h"
#define  _MEMCHECK_H_SIZE_OVER 100000
#include "memcheck.h"

int  readDataFile(PERSON **, char *);

/**
 *   Please note that main() is not a finished function.
 *   Although we said that you cannot modify codes at main() as a general rule,
 *   you may add your program codes at main() in this lab.
 */
int main(int argc, char **argv) {
    char *infile = "in.bin";    // default input binary data file 
    char *outfile = "out.txt";  // default output text data file
    PERSON *persons;      // array of personal data
    int num_persons;      // number of persons in the data file
    char *find[] = {"Andy", "Laura", "Joshua"};  // names of persons to search

    /**
     *  Get input/output file names from command line if given.
     *  The 1st argument is used as input file, and the 2nd as output.
     */
    if (argc > 1) {
        infile = argv[1];
        if (argc > 2) {
            outfile = argv[2];
            if (argc > 3) {
                printf("[WARNING] The 3rd and later arguments are ignored.\n");
            }
        }
    }

    /**
     *    Read binary data file "in.bin",
     *    which contains personal data of many persons
     */
    printf("- Reading binary data file, \"%s\"\n", infile);
    num_persons = readDataFile(&persons, infile);
    if (num_persons <= 0) {
        // DO NOT REMOVE THE FOLLOWING LINE. IF YOU DO, YOU GET 'F'.
        check_memory();
        return 1;
    }

    /**
     *    Dump the above personal data into a text file
     */
    printf("- Writing text data file, \"%s\"\n", outfile);

    /* ADD YOUR PROGRAM CODE HERE */


    /**
     *    Search the 3 persons in "find[]" array,
     *    and print to the information to console.
     *    The print format is the same as a line in "out.txt" 
     */
    for (int i = 0 ; i < 3 ; i++) {
        printf("- Searching \"%s\"\n", find[i]);

        /* ADD YOUR PROGRAM CODE HERE */

    }

    /*
     *    Compute average and standard deviation of ages of all persons
     *    in the data file. Also, compute average heights of males and
     *    females, respectively.
     */
    printf("- Computing average and std. deviation of ages, and heights\n");

    /* ADD YOUR PROGRAM CODE HERE */


    /**
     *    How many ladies live in "NewYork" ?
     */
    printf("- How many ladies live in \"NewYork\" ?\n");

    /* ADD YOUR PROGRAM CODE HERE */


    /**
     *    How many men have a phone number that begins with 777 ?
     */
    printf("- How many men have a phone that begins with 777 ?\n");

    /* ADD YOUR PROGRAM CODE HERE */


    /**
     *    How many ladies have her names that begins with 'E'?
     *    What are the most frequent phone prefix(es) which are owned
     *    by husbands of the above ladies?
     */
    char *msg = "- How many ladies have her name that begin with 'E'? "
                "And, phone prefix?";
    printf("%s\n", msg);

    /* ADD YOUR PROGRAM CODE HERE */


    /*
     *   DO NOT REMOVE THE FOLLOWING LINE.
     *   IF YOU DO, YOU GET 'F'.
     */
    check_memory();
    return 0;
} 

/*----- person.c -----*/
