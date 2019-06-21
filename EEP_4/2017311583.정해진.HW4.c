/**
 * Copyright (c) 2003-2018 by Hyoung Bok Min, All rights reserved.
 * Copyright (c) 2018 by Hae Jin Jung, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * Programming Lab.4 : Array of Strings and Dynamic memory allocation
 *
 * Summary of this lab :
 * A text file(infile.txt) is given that contains 6 students'
 * names, genders, scores of 4 subjects.
 * (Numbers 6 and 4 are read from the command line argument)
 * 1. Read this text file and store the contents
 *    and the average of the scores of the four subjects(Data type: double)
 *    in the binary file(binary.dat).
 * 2. Read the binary file you just saved, measure the size of the binary file,
 *    print it to the console using "printf()" function, and save the contents
 *    of the binary file back to the text file(outfile.txt).
 *
 * Written by Jung, Hae Jin,
 *             School of Electrical and Electronics Engineering
 *             Sungkyunkwan University.
 *
 * File name : 2017311583.JungHaeJin.HW4.c
 *
 * Written on September 01, 2011
 * Modification History :
 *    1. Wrtten by Hyoung Bok Min on August 19, 2012 (given as a template)
 *    2. modified by Hae Jin Jung on April 16, 2018
 *       (a) Add codes for reading and writing text file (homeworks).
 *
 * This is the main program provided to students who registered
 * in "EE Programming Lab" at School of Electrical Engineering,
 * Sungkyunkwan University.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#define  _MEMCHECK_H_SIZE_OVER 5000
#include "memcheck.h"

// THE FOLLOWING 6 LINES ARE GRADING PURPOSE ONLY. LEAVE AS IS.
#ifndef GRADE
#define GRADE
#endif
#ifdef GRADE
#include "grade.h"
#endif

int readTXT(char *input_file, int num_persons, int num_grades,
            char **name_array, char *gender_array, int **grades_array);
int writeBIN(char *binary_file, int num_persons, int num_grades,
            char **name_array, char *gender_array, int **grades_array);
int readBIN(char *binary_file, int num_persons, int num_grades,
            char **name_array, char *gender_array,
            int **grades_array, double *average_array);
int writeTXT(char *output_file, int num_persons, int num_grades,
            char **name_array, char *gender_array,
            int **grades_array, double *average_array);
void buildMemory(int num_persons, int num_grades,
            char ***name_array, char **gender_array,
            int ***grades_array, double **average_array);
void disposeMemory(int num_persons, int num_grades,
            char **name_array, char *gender_array,
            int **grades_array, double *average_array);


//======================================================================
// WARNINGS:
//   (1) : YOU MAY NOT MODIFY FUNCTIONS main() AND buildMemory()
//         UNLESS YOU CAN PROVE THAT THE GOAL OF THIS PROBLEM CANNOT
//         BE ACHIEVED WITHOUT MODIFICATION OF THOSE FUNCTIONS.
//   (2) : LEAVE ALL THE CODES BETWEEN #ifdef GRADE ... #endif AS IS.
//======================================================================
int main(int argc, char **argv) {
    int  get_positive_integer(char *number_string);
    char *input_file  = "infile.txt";   // input file name
    char *binary_file = "binary.dat";   // binary file name
    char *output_file = "outfile.txt";  // output file name
    int  num_persons, num_grades;
    char **name_array;      // array of names of size num_persons
    char *gender_array;     // array of genders of size num_persons
    int  **grades_array;    // array of arrays of gradess of size num_persons
    double *average_array;  // array of averages of size num_persons

    /**
     * Get the number of persons and the number of grades in the input file
     * from command line. The 1st argument is the number of persons, and
     * the 2nd argument is the number of grades.
     */
    if (argc < 3) {
        char *msg = "[ERROR] We need at least 2 positive integer arguments.\n"
             "[NOTE] The 1st argument is the number of persons in input file.\n"
             "[NOTE] The 2nd argument is the number of grades in input file.\n";
        char *usage = "\n"
             "usage: %s N1 N2 [in-file [bin-file [out-file]]]\n"
             "    N1 = number of persons in input text file\n"
             "    N2 = number of grades in input text file\n"
             "    in-file  = optional input text file name\n"
             "    bin-file = optional intermediate binary file name\n"
             "    out-file = optional output text file name\n";
        fprintf(stderr, "%s", msg);
        fprintf(stderr, usage, argv[0]);
        return -6;
    } else if (argc > 3) {
        // optional file names are given as arguments
        input_file = argv[3];
        if (argc > 4) {
            binary_file = argv[4];
            if (argc > 5) {
                output_file = argv[5];
                if (argc > 6) {
                    char *msg = "[WARNING] The 6th or later aguments "
                                "are ignored.";
                    fprintf(stderr, "%s\n", msg);
                }
            }
        }
    }

    num_persons = get_positive_integer(argv[1]);
    num_grades = get_positive_integer(argv[2]);
    if (num_persons < 0 || num_grades < 0)
        return -7;

#ifdef GRADE
    if (grade_init(num_persons, num_grades))
        return -5;
#endif

    /**
     * For every line of a text file (input_file = "infile.txt"),
     *    (a) Read a line of text file,
     *    (b) Compute average (double floating point number) of 4 numbers
     *             at every line entry, and
     *    (c) Write all the read data and the average numbers into
     *             a binary file (binary_file="binary.dat").
     *
     * Functions returns 1 if successful,
     *                   0 otherwise. (and gives error message)
     *
     * Note (1) : Only the data types of int, char, double are saved
     *            into the binary file.
     * Note (2) : For every line of input file data, the data should be written
     *            to binary file as the same sequence at text file, and then
     *            average of the line should be written.
     * Note (2) : Big-endian (optionally litte-endian) should be used
     *            in the binary data file.
     *            (Note that x86 is a little-endian CPU.)
     */
    printf("\nRead text file and write to binary file.\n");

    // Prepare memory for data storage
    buildMemory(num_persons, num_grades,
                &name_array, &gender_array, &grades_array, (double **)0);

    // Read data from text file into arrays.
    printf("Reading text file '%s'.\n", input_file);
    if (!readTXT(input_file, num_persons, num_grades,
                 name_array, gender_array, grades_array))
        return -1;
#ifdef GRADE
    // THE FOLLOWING IS GRADING PURPOSE ONLY, AND LEAVE IT AS IS.
    // YOU DO NOT NEED CODING FOR THE FOLLOWING FUNCTION.
    grade_txt_array(input_file, num_persons, num_grades,
                    name_array, gender_array, grades_array);
#endif

    // Write data in arrays to binary file.
    printf("Writing to binary file '%s'.\n", binary_file);
    if (!writeBIN(binary_file, num_persons, num_grades,
                  name_array, gender_array, grades_array))
        return -2;
#ifdef GRADE
    // THE FOLLOWING IS GRADING PURPOSE ONLY, AND LEAVE IT AS IS.
    // YOU DO NOT NEED CODING FOR THE FOLLOWING FUNCTION.
    grade_bin_file(binary_file, num_persons, num_grades,
                   name_array, gender_array, grades_array);
#endif

    // Dispose memories
    disposeMemory(num_persons, num_grades,
                  name_array, gender_array, grades_array, (double *)0);
    // DO NOT REMOVE THE FOLLOWING LINE.
    check_memory();

    /**
     * (a) Read a binary file ("binary.dat"),
     * (b) Print the size of the binary file to console, and
     * (c) Write all the read data (including the average numbers)
     *         into a text file ("outfile.txt").
     * (d) The text file should be the same as "infile.txt" except
     *         the double floating point number at the end of each line.
     * Functions returns 1 if successful,
     *                   0 otherwise. (and gives error message)
     */
    printf("\nRead binary file and write to text file.\n");

    // Prepare memory for data storage
    buildMemory(num_persons, num_grades,
                &name_array, &gender_array, &grades_array, &average_array);

    // Read binary file created by "writeBIN()", and put in data arrays.
    printf("Reading binary file '%s'.\n", binary_file);
    if (!readBIN(binary_file, num_persons, num_grades,
                 name_array, gender_array, grades_array, average_array))
        return -3;
#ifdef GRADE
    // THE FOLLOWING IS GRADING PURPOSE ONLY, AND LEAVE IT AS IS.
    // YOU DO NOT NEED CODING FOR THE FOLLOWING FUNCTION.
    grade_read_bin(binary_file, num_persons, num_grades,
                   name_array, gender_array, grades_array, average_array);
#endif

    // Write data at arrays to text file.
    printf("Writing to text file '%s'.\n", output_file);
    if (!writeTXT(output_file, num_persons, num_grades,
                  name_array, gender_array, grades_array, average_array))
        return -4;
#ifdef GRADE
    // THE FOLLOWING IS GRADING PURPOSE ONLY, AND LEAVE IT AS IS.
    // YOU DO NOT NEED CODING FOR THE FOLLOWING FUNCTION.
    grade_txt_file(output_file, num_persons, num_grades,
                   name_array, gender_array, grades_array, average_array);
#endif

    // Dispose memories
    disposeMemory(num_persons, num_grades,
                  name_array, gender_array, grades_array, average_array);
    // DO NOT REMOVE THE FOLLOWING LINE.
    check_memory();
    return 0;
}


/**
 * Given a string which represent a positive integer,
 * Return the positive integer for the string if it is valid,
 *        -1 if the string is invalid.
 */
int get_positive_integer(char *number_string) {
    int base = 10;    // decimal integer
    char *endptr;
    long value;

    errno = 0;    /* To distinguish success/failure after call */
    value = strtol(number_string, &endptr, base);

    /*
     * Check for various possible errors
     */
    if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
                         || (errno != 0 && value == 0)) {
        // out of range
        fprintf(stderr, "'%s' is too small or too large.\n", number_string);
        return -1;
    }

    if (endptr == number_string) {
        // not a number
        fprintf(stderr, "'%s' is not a number.\n", number_string);
        return -1;
    }

    /**
     *  If we got here, strtol() successfully parsed a number
     */
    if (*endptr != '\0') {
        // any character after number ?
        fprintf(stderr, "Further characters after number: %s\n", endptr);
        return -1;
    }
    if (value < 0) {
        // negative integer
        fprintf(stderr, "Number should be positive integer.\n");
        return -1;
    }
    if (value > INT_MAX) {
        // too large number
        fprintf(stderr, "%s is out of integer range.\n", number_string);
    }
    return (int)value;
}


/**
 * Given number of persons and number of grades at input file,
 * Build an array of names of size, num_persons,
 *       an array of genders of size, num_persons,
 *       an array of grades of size, num_persons,
 *       an array of averages of size, num_persons (this is optional).
 */
void buildMemory(int num_persons, int num_grades,
                 char ***p_name_array, char **p_gender_array,
                 int ***p_grades_array, double **p_average_array) {
    int index, gid;
    double *average_array;

    // Create memories
    char **name_array = (char **)mymalloc(num_persons*sizeof(char *));
    char *gender_array = (char *)mymalloc(num_persons*sizeof(char));
    int  **grades_array = (int **)mymalloc(num_persons*sizeof(int *));
    assert(name_array && gender_array && grades_array);
    if (p_average_array) {
        average_array = (double *)mymalloc(num_persons*sizeof(double));
        assert(average_array);
    }

    // Initialize name array and make grade memories to and array of arrays.
    for (index = 0 ; index < num_persons ; index++) {
        name_array[index] = NULL;
        gender_array[index] = (char)0;
        grades_array[index] = (int *)mymalloc(num_grades*sizeof(int));
        assert(grades_array[index]);
        for (gid = 0 ; gid < num_grades ; gid++)
            grades_array[index][gid] = 0;
        if (p_average_array)
            average_array[index] = 0.0;
    }

    // Output the built memories.
    *p_name_array = name_array;
    *p_gender_array = gender_array;
    *p_grades_array = grades_array;
    if (p_average_array)
        *p_average_array = average_array;
}


/**
 * Given an array of names,
 *       an array of genders,
 *       an array of grades,
 *       an array of averages (this may be NULL),
 * dispose the memories.
 */
void disposeMemory(int num_persons, int num_grades,
                   char **name_array, char *gender_array,
                   int **grades_array, double *average_array) {
    int index;

    // Dispose name_array memories
    for (index = 0; index < num_persons; index++) {
        myfree(name_array[index]);
    }
    myfree(name_array);

    // Dispose gender_array memories
    myfree(gender_array);

    // Dispose grades_array memories
    for (index = 0; index < num_persons; index++) {
        myfree(grades_array[index]);
    }
    myfree(grades_array);

    // Dispose name_array memories
    myfree(average_array);
}


/**
 * (a) Read a text file,
 * (b) Put the data into data arrays.
 *
 * Inputs :
 *       char *input_file : name of the input text file
 *       int num_persons  : number of persons
 *       int num_grades   : number of grades
 *
 * Output :
 *       char **name_array  : an array of names
 *       char *gender_array : an array of genders
 *       int **grades_array : an array of array of grades
 *
 *       Function returns 1 if successful,
 *                        0 otherwise. (and gives error message)
 */
int readTXT(char *input_file, int num_persons, int num_grades,
            char **name_array, char *gender_array, int **grades_array) {
    // open input file by text reading mode
    FILE *fpi;
    fpi = myfopen(input_file, "r");
    if (!fpi) {
        fprintf(stderr, "[ERROR] File '%s' not found.\n", input_file);
        return 0;
    }

    char *readLineBUF;    // string which saves a line as BUFSIZE as read
    int BUFSIZE = 200;    // expect value of size of reading line
    int NAMEBUF = 100;
    int index = 0, gid, pos = 0, nbytes;

    // allocate the memory for "read_line_buf"
    readLineBUF = (char *)mymalloc(BUFSIZE * sizeof(char));
    assert(readLineBUF);

    // read lines in "infile.txt" until going to last line
    while (fgets(readLineBUF, BUFSIZE, fpi)) {
    // check buffer overflow.
    // return value 0 if reading process do not execute successfully
        if (!strncmp(&readLineBUF[BUFSIZE], "\0", 1)) {
            myfclose(fpi);
            fprintf(stderr, "[ERROR] Overflow read line.\n");
            return 0;
        }
    // memory allocation of "name_array" saving name
        name_array[index] = (char *)mymalloc(NAMEBUF * sizeof(char));
        assert(name_array[index]);
    // check the input text file's names and genders
    // return value 0 if reading process do not execute successfully
        if (sscanf(readLineBUF, "%s %c%n",
            name_array[index], &gender_array[index], &nbytes) != 2) {
            myfclose(fpi);
            fprintf(stderr, "[ERROR] File read failure. ");
            fprintf(stderr, "One of name, sex at line ");
            fprintf(stderr, "%d of input file is invalid.\n", index + 1);
            return 0;
        }
        pos += nbytes;
    // check the input text file's grades of 4 subjects
    // return value 0 if reading process do not execute successfully
        for (gid = 0; gid < num_grades - 1; gid++) {
            if (sscanf(&readLineBUF[pos], "%d%n",
                       &grades_array[index][gid], &nbytes) != 1) {
                myfclose(fpi);
                fprintf(stderr, "[ERROR] File read failure. ");
                fprintf(stderr, "%dth grades at line ", gid + 1);
                fprintf(stderr, "%d of input file is invalid.\n", index + 1);
                return 0;
            } else {
                pos += nbytes;
            }
        }
        if (sscanf(&readLineBUF[pos], "%d",
                   &grades_array[index][gid]) != 1) {
            myfclose(fpi);
            fprintf(stderr, "[ERROR] File read failure. ");
            fprintf(stderr, "%dth grades at line ", gid + 1);
            fprintf(stderr, "%d of input file is invalid.\n", index + 1);
            return 0;
        }
        index++;
        pos = 0;    // initialization of variable
        if (index == num_persons) {  // go out the loop
            break;
        }
    }
    myfclose(fpi);
    // Free memory allocated for "readLineBUF"
    myfree(readLineBUF);
    // Return value 1 if function execute successfully
    return 1;
}

/**
 * Given arrays of names, genders, and array of grades,
 * (b) Compute average (double floating point number) of 4 numbers
 *      at every line entry, and
 * (c) Write all the read data and the average numbers into
 *     a binary file.
 *
 * Inputs :
 *       char *binary_file : name of the output binary file
 *       int num_persons   : number of persons
 *       int num_grades    : number of grades
 *       char **name_array  : an array of names
 *       char *gender_array : an array of genders
 *       int **grades_array : an array of array of grades
 *
 * Output :
 *       Function returns 1 if successful,
 *                        0 otherwise. (and gives error message)
 */
int writeBIN(char *binary_file, int num_persons, int num_grades,
             char **name_array, char *gender_array, int **grades_array) {
    // open binary file by binary writing mode
    FILE *fpb;
    fpb = myfopen(binary_file, "wb");
    if (!fpb) {
        myfclose(fpb);
        fprintf(stderr, "[ERROR] File '%s' not found.\n", binary_file);
        return 0;
    }

    int nameLength = 0;            // saving the length of name
    int num_sum_grades = 0;        // saving sum of grades of 4 subjects
    double num_aver_grades = 0.0;  // saving average of grades of 4 subjects

    for (int index = 0; index < num_persons; index++) {
        nameLength = strlen(name_array[index]);
    // check writing the length of name on binary file
    // return value 0 if writing process do not execute successfully
        if (fwrite(&nameLength, sizeof(int), 1, fpb) < 1) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to write length of ");
            fprintf(stderr, "%dth person's name in binary file.\n", index);
            return 0;
        }
    // check writing name on binary file
    // return value 0 if writing process do not execute successfully
        if (fwrite(name_array[index], sizeof(char), nameLength, fpb)
            < nameLength) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to write ");
            fprintf(stderr, "%dth person's name in binary file.\n", index);
            return 0;
        }
    // check writing gender on binary file
    // return value 0 if reading process do not execute successfully
        if (fwrite(&gender_array[index], sizeof(char), 1, fpb) < 1) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to write ");
            fprintf(stderr, "%dth person's sex in binary file.\n", index);
            return 0;
        }
    // check writing grades of 4 subjects on binary file
    // return value 0 if writing process do not execute successfully
        for (int gid = 0; gid < num_grades; gid++) {
            if (fwrite(&grades_array[index][gid], sizeof(int), 1, fpb) < 1) {
                myfclose(fpb);
                fprintf(stderr, "[ERROR] Fail to write %dth person's ", index);
                fprintf(stderr, "%dth grade in binary file.\n", gid + 1);
                return 0;
            }
            num_sum_grades += *(*(grades_array + index) + gid);
        }
        num_aver_grades = (double)num_sum_grades / (double)num_grades;
    // check writing the average of grades of 4 subjects on binary file
    // return value 0 if writing process do not execute successfully
        if (fwrite(&num_aver_grades, sizeof(double), 1, fpb) < 1) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to write average of ");
            fprintf(stderr, "%dth person's grades in binary file.\n", index);
            return 0;
        }
    // initialization of variables
        nameLength = 0;
        num_sum_grades = 0;
        num_aver_grades = 0.0;
    }

    myfclose(fpb);
    // Return value 1 if program execute successfully
    return 1;
}

/**
 * (a) Read a binary file,
 * (b) Compute size of the binary file, print the size, and
 * (c) put the data to data arrays.
 *
 * Inputs :
 *       char *binary_file : name of the input binary file
 *       int num_persons   : number of persons
 *       int num_grades    : number of grades
 *
 * Output :
 *       char **name_array  : an array of names
 *       char *gender_array : an array of genders
 *       int **grades_array : an array of array of grades
 *       double *average_array : an array of averages
 *
 *       Function returns 1 if successful,
 *                        0 otherwise. (and gives error message)
 */
int readBIN(char *binary_file, int num_persons, int num_grades,
            char **name_array, char *gender_array,
            int **grades_array, double *average_array) {
    // open binary file by binary reading mode
    FILE *fpb;
    fpb = myfopen(binary_file, "rb");
    if (!fpb) {
        fprintf(stderr, "[ERROR] File '%s' not found.\n", binary_file);
        myfclose(fpb);
        return 0;
    }

    // move file pointer to the end of binary file
    fseek(fpb, 0L, SEEK_END);

    // compute size of the binary file and print the size
    long file_size = ftell(fpb);
    // check computing size of the binary file
    // return value 0 if moving pointer process do not execute successfully
    if (!(int)ftell(fpb)) {
        myfclose(fpb);
        fprintf(stderr, "[ERROR] Fail to compute the size of ");
        fprintf(stderr, "file '%s'\n", binary_file);
        return 0;
    }
    printf("Size = %ld\n", file_size);

    // rewinding file pointer
    // return value 0 if rewinding process do not execute successfully
    if (fseek(fpb, 0L, SEEK_SET)) {
        myfclose(fpb);
        fprintf(stderr, "[ERROR] File rewinding fails.\n");
        return 0;
    }

    int nameLength = 0;

    for (int index = 0; index < num_persons; index++) {
    // check writing the length of name on binary file
    // return value 0 if reading process do not execute successfully
        if (fread(&nameLength, sizeof(int), 1, fpb) < 1) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to read length of ");
            fprintf(stderr, "%dth person's name in binary file.\n", index);
            return 0;
        }

        name_array[index] = (char *)mymalloc((nameLength + 1) * sizeof(char));
        assert(name_array[index]);
        name_array[index][nameLength] = (char)0;
    // check writing the length of name on binary file
    // return value 0 if reading process do not execute successfully
        if (fread(name_array[index], sizeof(char), nameLength, fpb)
            < nameLength) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to read ");
            fprintf(stderr, "%dth person's name in binary file.\n", index);
            return 0;
        }
    // check writing the length of name on binary file
    // return value 0 if reading process do not execute successfully
        if (fread(&gender_array[index], sizeof(char), 1, fpb) < 1) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to read ");
            fprintf(stderr, "%dth person's sex in binary file.\n", index);
            return 0;
        }
    // check writing the length of name on binary file
    // return value 0 if reading process do not execute successfully
        for (int gid = 0; gid < num_grades; gid++) {
            if (fread(&grades_array[index][gid], sizeof(int), 1, fpb) < 1) {
                myfclose(fpb);
                fprintf(stderr, "[ERROR] Fail to read %dth person's ", index);
                fprintf(stderr, "%dth grade in binary file.\n", gid + 1);
                return 0;
            }
        }
    // check writing the length of name on binary file
    // return value 0 if reading process do not execute successfully
        if (fread(&average_array[index], sizeof(double), 1, fpb) < 1) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] Fail to read average of ");
            fprintf(stderr, "%dth person's grades in binary file.\n", index);
            return 0;
        }
        nameLength = 0;
    }
    myfclose(fpb);
    // Return value 1 if program execute successfully
    return 1;
}


/**
 * Given array of names, genders, grades, and averages,
 * Write all the read data and the average numbers into a text file.
 *
 * Inputs :
 *       char *output_file : name of the output text file
 *       int num_persons   : number of persons
 *       int num_grades    : number of grades
 *       char **name_array  : an array of names
 *       char *gender_array : an array of genders
 *       int **grades_array : an array of array of grades
 *       double *average_array : an array of averages
 *
 * Output :
 *       Function returns 1 if successful,
 *                        0 otherwise. (and gives error message)
 */
int writeTXT(char *output_file, int num_persons, int num_grades,
             char **name_array, char *gender_array,
             int **grades_array, double *average_array) {
    // open output file by text writing mode
    FILE *fpo;
    fpo = myfopen(output_file, "w");
    if (!fpo) {
        fprintf(stderr, "[ERROR] File '%s' cannot be created.\n", output_file);
        return 0;
    }

    for (int index = 0; index < num_persons; index++) {
    // write name and gender on output file
        fprintf(fpo, "%s %c ", name_array[index], gender_array[index]);
    // write grades of 4 subjects on output file
        for (int gid = 0; gid < num_grades; gid++) {
            fprintf(fpo, "%d ", grades_array[index][gid]);
        }
    // write average of grades on output file
        fprintf(fpo, "%.2lf", average_array[index]);
        if (index < num_persons - 1) {
            fprintf(fpo, "\n");
        }
    }

    myfclose(fpo);
    // Return value 1 if program execute successfully
    return 1;
}

/*---------- string.c ----------*/
