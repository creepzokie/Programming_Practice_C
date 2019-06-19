/**
 *  Copyright (c) 2003-2017 by Hyoung Bok Min, All rights reserved.
 *  Copyright (C) 2018 by Hae Jin Jung, All rights reserved.
 *  For license information, please refer to
 *      http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *  Lab 2 of C Programming : Handling Text Files
 *
 *  Summary of this lab :
 *  Measure the size of a text file and output it to the console screen.
 *  - Read a text file, and calculate the average
 *    of all students' N subject points as a double floating point number
 *    and output to the console up to the second decimal place.
 *  - Output the number of M persons to the console by sex.
 *  - Print the average of M ages to the first decimal place on the console.
 *  - Simultaneously output "outfile.txt" to the console.
 *
 *  Written by Jung, Hae Jin,
 *             School of Electrical and Electronics Engineering
 *             Sungkyunkwan University.
 *
 *  File name : 2017311583.JungHaeJin.HW2.c
 *
 *  Written on September 01, 2011
 *  Modification History :
 *     1. Wrtten by Hyoung Bok Min on August 19, 2012 (given as a template)
 *     2. modified by Hae Jin Jung on April 2, 2018
 *        (a) Add codes for reading and writing text file (homeworks).
 *
 *  This is the main program provided to students who registered
 *  in "Programming Lab" at School of Information and Communication,
 *  Sungkyunkwan University.
 *
 *  ==========================
 *       WARNING
 *  ==========================
 *  DO NOT MODIFY function main(). You can add or delete comments in main(),
 *  But, you cannot add or delete program codes (statements) in main().
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

/**
 *   Given two file pointers which are about "infile.txt" and "outfile.txt",
 *   print size of "infile.txt" to stdout and "outfile.txt".
 *
 *   Outputs:
 *       file_size : size of "infile.txt".
 *
 *   Inputs:
 *       *fpi : pointer of "infile.txt".
 *       *fpo : pointer of "outfile.txt".
 */

int computeSize(FILE *fpi, FILE *fpo) {
    fseek(fpi, 0L, SEEK_END);

    long file_size = ftell(fpi);
    if (!(int)ftell(fpi)) {
        return 0;
    }
    printf("\nComputing size of input file.\n");
    printf("Size of input file = %ld\n", file_size);

    fprintf(fpo, "\n");
    fprintf(fpo, "Computing size of input file.\n");
    fprintf(fpo, "Size of input file = %ld\n", file_size);

    return 1;
}

/**
 *   Given two file pointers which are about "infile.txt" and "outfile.txt",
 *   print size of "infile.txt" to stdout and "outfile.txt".
 *
 *   Outputs:
 *       name         : name of "infile.txt"
 *       aveGrade     : average of grades of each student in "infile.txt"
 *       lineNums     : line number of "infile.txt"
 *       gender       : name of "infile.txt"
 *       subjNums     : the number of subjects in "infile.txt"
 *       validLNums   : the number of valid line which is match form
 *                      of reading file "infile.txt"
 *       invalidLNums : the number of invalid line which is not match form
 *                      of reading file "infile.txt"
 *       maleNums     : the number of males in "infile.txt"
 *       femaleNums   : the number of females in "infile.txt"
 *       averAges     : average of ages of each student in "infile.txt"
 *       file_size    : size of "infile.txt".
 *
 *   Inputs:
 *       *fpi : pointer of "infile.txt".
 *       *fpo : pointer of "outfile.txt".
 */

int readFile(FILE *fpi, FILE *fpo) {
    char *read_line_buf;    // string which saves a line as BUFSIZE as read
    int BUFSIZE = 1024;     // expect value of size of reading line

    int age;
    int score[4];           // a list of grades of subjects
    char gender, name[10];

    int lineNums = 0, validLNums = 0, invalidLNums = 0;

    int sumGrades = 0;      // sum of grades of each person in "infile.txt"
    double aveGrade = 0.0;

    int studNums = 0;       // the number of students in "infile.txt"
    int subjNums = 0;
    int maleNums = 0, femaleNums = 0;

    int sumAges = 0;        // sum of ages of each person in "infile.txt"
    double averAges = 0.0;

    // print on console
    printf("\nRead text file.\n");
    // print on "outfile.txt"
    fprintf(fpo, "\n");
    fprintf(fpo, "Read text file.\n");

    // allocate the memory for "read_line_buf"
    read_line_buf = (char *)malloc(BUFSIZE * sizeof(char));
    assert(read_line_buf);

    // read lines in "infile.txt" until going to last line
    while (fgets(read_line_buf, BUFSIZE, fpi)) {
    // return value 0 if reading process do not execute successfully
        if (!strncmp(&read_line_buf[BUFSIZE], "\0", 1)) {
            return 0;
        }
        sscanf(read_line_buf, "%s", name);
    // if "fgets" function read the line start with "//" or else
        if (strncmp(name, "//", 2)) {
    // if "fgets" function read the line start with the number or else
            if (strlen(name) == 1) {
    // "fgets" function read the line start with the number, then execute.
                sscanf(read_line_buf, "%d%d", &studNums, &subjNums);
                lineNums++;
            } else {
    // "fgets" function read the line start with the character, then execute.
    // Did "infile.txt" have "name", "age", "gender" properly?
                if (sscanf(read_line_buf, "%s %d %c", name, &age, &gender)
                    == 3) {
    // Did "infile.txt" have grades as the number of subjects as properly?
                    if (sscanf(read_line_buf, "%s %d %c %d%d%d%d",
                        name, &age, &gender,
                        &score[0], &score[1], &score[2], &score[3])
                        == 3 + subjNums) {
    // Did "infile.txt" have sex properly(only "M" or "F"?)?
                        if (strncmp(&gender, "M", 1)
                            && strncmp(&gender, "F", 1)) {
                            lineNums++;
                            invalidLNums++;
    // print on console
                            printf("[ERROR] Gender at line %d ", lineNums);
                            printf("of input file is '%c'.\n", gender);
    // print on "outfile.txt"
                            fprintf(fpo, "[ERROR] Gender at line");
                            fprintf(fpo, " %d of input file ", lineNums);
                            fprintf(fpo, "is '%c'.\n", gender);
                        } else {
                            if (strncmp(&gender, "M", 1) == 0) {
    // maleNums plus 1 when gender "M"
                                maleNums++;
                                }
                            if (strncmp(&gender, "F", 1) == 0) {
    // femaleNums plus 1 when gender "F"
                                femaleNums++;
                                }
    // sum grades saved in "score"
                            for (int i = 0; i < subjNums; i++) {
                                sumGrades += score[i];
                                }
                            aveGrade = (double)sumGrades /(double)subjNums;
    // print on console
                            printf("Average of grades of %s ", &name);
                            printf("= %.2lf\n", aveGrade);
    // print on "outfile.txt"
                            fprintf(fpo, "Average of grades of %s ", name);
                            fprintf(fpo, "= %.2lf\n", aveGrade);
    // read line successful
    // modify several values, count lines and valid lines
                            sumGrades = 0;
                            sumAges += age;
                            validLNums++;
                            lineNums++;
                        }
                    } else {
    // "infile.txt" don't have grades as the number of subjects as properly.
    // count lines and invalid lines
                        lineNums++;
                        invalidLNums++;
    // print on console
                        printf("[ERROR] %d-th grade ", subjNums);
                        printf("at line %d of input file ", lineNums);
                        printf("is invalid.\n");
    // print on "outfile.txt"
                        fprintf(fpo, "[ERROR] %d-th grade ", subjNums);
                        fprintf(fpo, "at line %d", lineNums);
                        fprintf(fpo, " of input file is invalid.\n");
                    }
                } else {
    // "infile.txt" don't have "name", "age", "gender" properly.
    // count lines and invalid lines
                    lineNums++;
                    invalidLNums++;
    // print on console
                    printf("[ERROR] One of name, age, sex ");
                    printf("at line %d is invalid.\n", lineNums);
    // print on "outfile.txt"
                    fprintf(fpo, "[ERROR] One of name, age, sex ");
                    fprintf(fpo, "at line %d is invalid.\n", lineNums);
                }
            }
        } else {
    // count lines
            lineNums++;
        }
    }
    // if the number of invalid line is more than 0, then execute
    if (invalidLNums != 0) {
    // print on console
        printf("[ERROR] Input file contains %d valid lines ", validLNums);
        printf("(%d expected).\n", validLNums + invalidLNums);
    // print on "outfile.txt"
        fprintf(fpo,
                "[ERROR] Input file contains %d valid lines (%d expected).\n",
                validLNums, validLNums + invalidLNums);
    }
    // calculate average of ages of student in "infile.txt"
    averAges = (double)sumAges / (double)(studNums - invalidLNums);
    // print on console
    printf("Number of males = %d\n", maleNums);
    printf("Number of females = %d\n", femaleNums);
    printf("Average of ages = %.1lf\n", averAges);
    // print on "outfile.txt"
    fprintf(fpo, "Number of males = %d\n", maleNums);
    fprintf(fpo, "Number of females = %d\n", femaleNums);
    fprintf(fpo, "Average of ages = %.1lf\0", averAges);
    // free memory allocated for "read_line_buf"
    free(read_line_buf);
    // return value 1 if program execute successfully
    return 1;
}


/*------------------  textfile.c ----------------------*/
