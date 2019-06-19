/**
 * Copyright (c) 2003-2017 by Hyoung Bok Min, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * Programming Lab. : Handling Text Files
 *
 * This is the main program provided to students who registered
 * in "Programming Lab" at School of Information and Communication,
 * Sungkyunkwan University.
 *
 * ==========================
 *      WARNING
 * ==========================
 * DO NOT MODIFY function main(). You can add or delete comments in main(),
 * But, you cannot add or delete program codes (statements) in main().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// macros to print to both screen and file.
#define PRINTF(fp, ...)             \
    {                               \
        printf(__VA_ARGS__);        \
        fprintf(fp, __VA_ARGS__);   \
    }
#define CLOSE2(fp1, fp2)   \
    {                      \
        fclose(fp1);       \
        fclose(fp2);       \
    }

/**
 * NOTE : You can change function prototypes of the followings if you need.
 */
int computeSize(FILE *fpi, FILE *fpo);
int readFile(FILE *fpi, FILE *fpo);

/*---< infile.txt >-----
// lines that begin with 2 consecutive slashes are comments.
9   4      // number of students, and number of classes
// All the following lines are supposed to be "name age sex multiple-grades"
// of a person
// If any line does not match the line format, give error message and skip.
slee 29 M 87 59 69 25
hPark 18 F 34 97 81 78
YKim 21 F 29 33 8 68
// note that the following line contains an error
dKim 25 M 51 16 28
dKim 25 M 51 16 28 32
// the following line also contains an error
kog F 17 8 5 45
kog 20 F 17 8 5 45
// the following line also contains an error
lee 21 K 78 3 89 92
lee 21 M 78 3 89 92
------------------------*/


int main(void) {
    char *input_file  = "infile.txt";   // input file name
    char *output_file = "outfile.txt";  // output file name
    // WARNING : DO NOT CHANGE ABOVE FILE NAMES
    FILE *fpi, *fpo;

    /**
     * Open input text file.
     */
    fpi = fopen(input_file, "r");
    if (!fpi) {
        fprintf(stderr, "[ERROR] File '%s' not found.\n", input_file);
        return 1;
    }
    fpo = fopen(output_file, "w");
    if (!fpo) {
        fclose(fpi);
        fprintf(stderr, "[ERROR] File '%s' cannot be created.\n", output_file);
        return 2;
    }

    /**
     * Compute size of input text file.
     * This function returns 1 if the computing is successful, and
     *     returns 0 if there is any reason why computing cannot be performed.
     * Use function "fseek()" and function "ftell()"
     */

    int computeSize(fpi, fpo) {
        fseek(fpi, 0, SEEK_END);
        long file_size = ftell(fpi);
        if (!(int)ftell(fpi)) {
            return 0;
        }
        fclose(fpi);
        printf("Size of %s: %ld bytes.\n", input_file, file_size);
        return 1;
    }

    if (!computeSize(fpi, fpo)) {
        CLOSE2(fpi, fpo)
        return 3;
    }

    /**
     * Rewind the input file pointer to read the text file from the beginning.
     */
    if (fseek(fpi, 0L, SEEK_SET)) {
        PRINTF(fpo, "[ERROR] file rewinding fails.\n")
        CLOSE2(fpi, fpo)
        return 4;
    }

    /**
     * Read text file and compute grade averages for each person.
     * This function returns 1 if the reading can be finished, and
     *     returns 0 if there is any reason why reading cannot be finished.
     * Use function "fgets()" and function "sscanf()"
     */

    int readFile(fpi, fpo) {



    }

    if (!readFile(fpi, fpo)) {
        CLOSE2(fpi, fpo)
        return 5;
    }

    /**
     * Close files
     */
    CLOSE2(fpi, fpo)
    return 0;
}

/*------------------  textfile.c ----------------------*/
