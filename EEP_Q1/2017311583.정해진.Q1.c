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

char **data_file(char *infile,
                 char *outfile,
                 COMP_TYPE **comptype_array,
                 int *num_comps) {
    FILE *fpb;
    fpb = myfopen(infile, "rb");
    if (!fpb) {
        fclose(fpb);
        fprintf(stderr, "[ERROR] File '%s' not found.\n", infile);
        return 0;
    }
    FILE *fpo;
    fpo = myfopen(outfile, "w");
    if (!fpo) {
        fclose(fpb);
        fprintf(stderr, "[ERROR] File '%s' cannot be created.\n", outfile);
        return 0;
    }

    if (fread(&num_comps, sizeof(int), 1, fpb) == 0) {
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, "'%s' may not have a number of components.\n", infile);
    } else {
        fprintf(fpo, "%d\n", num_comps);
    }
    //printf("(%ld)\n", ftell(fpb));

    unsigned short nameLength;
    char nameOne[1];
    COMP_TYPE compTYPE;
    double value;
    unsigned short minusNums;
    char minusPin[1];
    unsigned short minusLength;
    char minusNameOne[1];
    unsigned short plusNums;
    char plusPin[1];
    unsigned short plusLength;
    char plusNameOne[1];


    comptype_array = (COMP_TYPE *)mymalloc(4);
    for (int index = 0; index < num_comps; index++) {
        nameLength = 0;
        if (fread(&nameLength, sizeof(unsigned short), 1, fpb) == 0) {
            fprintf(stderr, "[ERROR]\n");
        }
        //printf("(%ld)\n", ftell(fpb));
        *nameOne = (char)0;
        for (int nameindex = 0; nameindex < nameLength; nameindex++) {
            if (fread(nameOne, sizeof(char), 1, fpb) != 0) {
                fprintf(fpo, "%c", nameOne[0]);
            }
        }
        //printf("(%ld)\n", ftell(fpb));
        if (fread(&compTYPE, sizeof(COMP_TYPE), 1, fpb) == 0) {
            fprintf(stderr, "[ERROR]\n");
        } else {
            if (compTYPE == 0) {
                comptype_array[0]++;
                fprintf(fpo, " %s", "R");
            }
            if (compTYPE == 1) {
                comptype_array[1]++;
                fprintf(fpo, " %s", "C");
            }
            if (compTYPE == 2) {
                comptype_array[2]++;
                fprintf(fpo, " %s", "B");
            }
            if (compTYPE == 3) {
                comptype_array[3]++;
                fprintf(fpo, " %s", "L");
            }
        }
        //printf("(%ld)\n", ftell(fpb));
        value = 0.0;
        if (fread(&value, sizeof(double), 1, fpb) != 0) {
            printf(" %.1f", value);
            fprintf(fpo, " %.1f", value);
        } else {
            fprintf(stderr, "[ERROR]1\n");
        }
        //printf("(%ld)\n", ftell(fpb));

        minusNums = 0;
        if (fread(&minusNums, sizeof(unsigned short), 1, fpb) == 0) {
            fprintf(stderr, "[ERROR]2\n");
        } else {
            printf(" %u ", minusNums);
            fprintf(fpo, " %u ", minusNums);
        }
        //printf("(%ld)\n", ftell(fpb));
        for (int index2 = 0; index2 < minusNums; index2++) {
            *minusPin = (char)0;
            if (fread(minusPin, sizeof(char), 1, fpb) != 0) {
                printf("%c", minusPin[0]);
                fprintf(fpo, "%c", minusPin[0]);
            }
            //printf("(%ld)\n", ftell(fpb));
            minusLength = 0;
            if (fread(&minusLength, sizeof(unsigned short), 1, fpb) == 0) {
                printf(" %u ", minusLength);
                fprintf(stderr, "[ERROR]3\n");
            }
            //printf("(%ld)\n", ftell(fpb));
            *minusNameOne = (char)0;
            for (int minusindex = 0; minusindex < minusLength; minusindex++) {
                if (fread(minusNameOne, sizeof(char), 1, fpb) != 0) {
                    fprintf(fpo, "%c", minusNameOne[0]);
                }
            }
            //printf("(%ld)\n", ftell(fpb));
            fprintf(fpo, " ");
        }

        plusNums = 0;
        if (fread(&plusNums, sizeof(unsigned short), 1, fpb) == 0) {
            fprintf(stderr, "[ERROR]\n");
        } else {
            fprintf(fpo, " %u ", plusNums);
        }
        for (int index3 = 0; index3 < plusNums; index3++) {
            *plusPin = (char)0;
            if (fread(plusPin, sizeof(char), 1, fpb) != 0) {
                fprintf(fpo, "%c", plusPin[0]);
            }
            plusLength = 0;
            if (fread(&plusLength, sizeof(unsigned short), 1, fpb) == 0) {
                fprintf(stderr, "[ERROR]\n");
            }
            *plusNameOne = (char)0;
            for (int plusindex = 0; plusindex < plusLength; plusindex++) {
                if (fread(plusNameOne, sizeof(char), 1, fpb) != 0) {
                    fprintf(fpo, "%c", plusNameOne[0]);
                }
            }
            fprintf(fpo, " ");
        }
        fprintf(fpo, "\n");
    }
    myfclose(fpb);
    myfclose(fpo);
}

int count_num_comps(COMP_TYPE *comptype_array, int num_comps, int *numr, int *numc, int *numl) {

}

void list_name(COMP_TYPE *comptype_array, char **name_array, int num_comps) {

}

void dispose_memory(char **name_array, COMP_TYPE *comptype_array, int num_comps) {
    for (int i = 0; i < num_comps; i++) {
        myfree(name_array[i]);
    }
    myfree(name_array);
    myfree(comptype_array);
}

/*------ q1.c ------*/
