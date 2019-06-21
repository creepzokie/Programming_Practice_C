/**
 *  Copyright (c) 2003-2017 by Hyoung Bok Min, All rights reserved.
 *  Copyright (C) 2018 by Hae Jin Jung, All rights reserved.
 *  For license information, please refer to
 *      http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *  complier used : GNU GCC Compiler 7.2
 *
 *  Lab 9 of C Programming : Recursion
 *
 *  Summary of this lab :
 *  1. "recursion.c" is given
 *  2. Following functions must be written at lab hours
 *     - int iterativeFactorial(int n)
 *     - int recursiveFactorial(int n)
 *     - void reverse(char *string)
 *     - int hanoi(int ndisks, int from, int to, int via,
 *                 int *farr, int *tarr, int size);
 *       (function returns the number of disk moves)
 *
 *  Written by Jung, Hae Jin,
 *             School of Electrical and Electronics Engineering
 *             Sungkyunkwan University.
 *
 *  File name : 2017311583.JungHaeJin.HW9.c
 *
 *  Written on September 01, 2011
 *  Modification History :
 *     1. Wrtten by Hyoung Bok Min on August 19, 2012 (given as a template)
 *     2. modified by Hae Jin Jung on June 12, 2018
 *        (a) Add codes for reading and writing text file (homeworks).
 *
 *  This is the main program provided to students who registered
 *  in "Programming Lab" at School of Information and Communication,
 *  Sungkyunkwan University.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define BUFSIZE    128
#define JS_FILE    "hanoi_sol.js"
#define FPRINTF(fp, ...)  \
        if (fprintf(fp, __VA_ARGS__) < 1) {  \
            fprintf(stderr, "[ERROR] fail to write to a file\n"); \
            exit(100); \
        }

// =========================================================================
// Functions you have to write
//
// PLEASE WRITE YOUR FUNCTIONS HERE BY MODIFYING FUNCTION PROTOTYPES BELOW.
//
// =========================================================================
int iterativeFactorial(int number);
int recursiveFactorial(int number);
void reverse(char *str);
int hanoi(int ndisks, int from, int to, int via, int *farr, int *tarr,
          int size);

// Given Functions
int allWhites(char *str);
int ipow(int base, int expon);
int writeJS(FILE *fp, int ndisks, int *farr, int *tarr, int size, int to_tower);


int main(void) {
    int  number, nitems, sumi, sumr, okay = 0;
    char string[BUFSIZE];
    FILE *fp;
    int *farr, *tarr, size, retsize, ndisks;

    //
    // Problem 1 : Computing factorial
    //
    while (!okay) {
        printf("\nEnter a positive integer for factorial : ");
        if (fgets(string, BUFSIZE, stdin))  {
            nitems = sscanf(string, "%d", &number);
            if (nitems == 1 && number > 0)
                okay = 1;
        }
        if (!okay)
            printf("[ERROR] Need a positive integer\n");
    }
    sumi = iterativeFactorial(number);
    sumr = recursiveFactorial(number);
    printf("%d! = %d, %d\n", number, sumi, sumr);

    //
    // Problem 2 : Reverse a string
    //
    okay = 0;
    while (!okay) {
        printf("\nEnter a string for reversal : ");
        if (fgets(string, BUFSIZE, stdin)) {
            if (!allWhites(string))
                okay = 1;
        }
        if (!okay)
            printf("[ERROR] Need a string\n");
    }
    reverse(string);
    printf("\n");

    //
    // Problem 3 : Tower of Hanoi
    //
    okay = 0;
    while (!okay) {
        printf("\nEnter a positive integer for number of disks : ");
        if (fgets(string, BUFSIZE, stdin))  {
            nitems = sscanf(string, "%d", &ndisks);
            if (nitems == 1 && ndisks > 0 && ndisks <= 10)
                okay = 1;
        }
        if (!okay)
            printf("[ERROR] Need a positive integer between 1 to 10.\n");
    }
    // Prepare memory : 2**ndisks - 1
    size = ipow(2, ndisks) - 1;
    farr = (int *)malloc(size*sizeof(int));
    tarr = (int *)malloc(size*sizeof(int));
    assert(farr && tarr);

    // Solve hanoi problem : move ndisks from tower 1 to tower 3 via tower 2
    // Record tower numbers to arrays 'farr' and 'tarr' for disk movement.
    // You may change function prototype of this function if you really need.
    // Be careful so that index of the arrays does not exceed 'size'.
    // This function returns the number of moves, i.e., actual size of
    //   'farr' and 'tarr'.
    printf("Solving Tower of Hanoi problem of %d disks\n", ndisks);
    retsize = hanoi(ndisks, 1, 3, 2, farr, tarr, size);
    assert(retsize == size);

    // Write JavaScript file for viewing
    printf("Writing JavaScript file '%s' for Hanoi problem\n", JS_FILE);
    fp = fopen(JS_FILE, "w");
    if (!fp) {
        fprintf(stderr, "Fail to open file '%s'.\n", JS_FILE);
        return 1;
    }
    if (!writeJS(fp, ndisks, farr, tarr, size, 3)) {
        fprintf(stderr, "Fail to write file '%s'.\n", JS_FILE);
        return 2;
    }
    printf("Place '%s' at the folder with hanoi.html and hanoi.js,\n", JS_FILE);
    printf("and open hanoi.html with your web browser to view the solution.\n");
    fclose(fp);
    free(farr);
    free(tarr);
    return 0;
}


/**
 *    Given a string (char *str),
 *    Returns
 *      1 if all the characters in the string are white space characters,
 *      0 otherwise.
 *
 *    Side Effect : Removes new line character at the end of the string
 */
int allWhites(char *str) {
    int all_white = 1;

    while (*str) {
        if (*str == '\n' || *str == '\r') {
            *str = (char)0;
            break;
        } else if (!isspace(*str)) {
            all_white = 0;
        }
        str++;
    }
    return all_white;
}


/**
 *    Compute 'base' to the power of 'expon', i.e., base**expon.
 *    This function is integer version of function "pow()" in math library.
 *    Please note that the "pow()" in math library use double floating point
 *    numbers for its inputs and output.
 *
 *    Inputs:
 *        int base : base number (e.g., 3 for 3**8)
 *        int expon: exponent number (e.g., 8 for 3**8)
 *
 *    Outputs:
 &        Returns 'base' to the power of 'expon'.
 */
int ipow(int base, int expon) {
    int result = 1;
    while (expon) {
        if (expon & 1)
            result *= base;
        expon >>= 1;
        base *= base;
    }

    return result;
}


/**
 *    Given disk movement data of tower of hanoi problem,
 *    Generate JavaScript file to view the movment.
 *
 *    Function returns 1 if successfuly written into file,
 *                     0 otherwise.
 *
 *    NOTE : This function assumes that 'from-tower' is 1.
 */
int writeJS(
    FILE *fp,     // input:  file into which JavaScript code is written
    int ndisks,   // input:  number of disks at tower
    int *farr,    // output: disk moves from this tower (1, 2, or 3)
    int *tarr,    // output: disk moves to this tower (1, 2, or 3)
    int size,     // input:  size of the above two array
    int to_tower  // input:  tower 2 or 3 to which all disks should be moved
    ) {
    int  index;   // array index
    time_t now;   // to print time
    int  is_okay = 1;                   // error status
    int  disks_at[4] = { 0, 0, 0, 0 };  // number of disks at 3 towers

    /**
     * Check validity of input parameters
     */
    if (size < 1) {
        fprintf(stderr, "size of farr/tarr array should be 1 or larger.\n");
        return 0;
    }
    if (ndisks < 1 || ndisks > 10) {
        fprintf(stderr, "writeJS is valid only for 10 or less disks\n");
        return 0;
    }
    if (to_tower < 2 || to_tower > 3) {
        fprintf(stderr, "to-tower number should be 2 or 3 ");
        fprintf(stderr, "while given as %d\n", to_tower);
        is_okay = 0;
    }
    if (farr[0] != 1) {
        fprintf(stderr, "Tower number of from-tower must be 1, i.e.,");
        fprintf(stderr, " farr[0] must be 1.\n");
        is_okay = 0;
    }
    if (!is_okay)
        return 0;
    disks_at[1] = ndisks;
    for (index = 0 ; index < size ; index++) {
        if (farr[index] < 1 || farr[index] > 3) {
            fprintf(stderr, "farr[%d] number should be 1, 2, or 3 ", index);
            fprintf(stderr, "while given as %d\n", farr[index]);
            is_okay = 0;
        }
        if (tarr[index] < 1 || tarr[index] > 3) {
            fprintf(stderr, "tarr[%d] number should be 1, 2, or 3 ", index);
            fprintf(stderr, "while given as %d\n", tarr[index]);
            is_okay = 0;
        }
        if (farr[index] == tarr[index]) {
            fprintf(stderr, "farr[%d] should be different from ", index);
            fprintf(stderr, "tarr[%d].\n", index);
            is_okay = 0;
        }
        if (is_okay) {
            if (disks_at[farr[index]] <= 0) {
                fprintf(stderr, "There is no disk at tower %d", farr[index]);
                fprintf(stderr, " at step %d.\n", index);
                is_okay = 0;
            }
            (disks_at[farr[index]])--;
            (disks_at[tarr[index]])++;
        }
    }
    if (!is_okay)
        return 0;

    /**
     * Write JavaScript code for information on movement of disks
     */
    FPRINTF(fp, "// File name : %s\n", JS_FILE);
    FPRINTF(fp, "// Solution of Tower of Hanoi in JavaScript\n");
    now = time(NULL);
    FPRINTF(fp, "// Generated by \"%s\" on %s", __FILE__, ctime(&now));
    FPRINTF(fp, "var ndisks = %d;   // number of disks\n", ndisks);
    FPRINTF(fp, "var to_tower = %d; // to-tower number\n", to_tower);
    FPRINTF(fp, "var fromT = [ %d", farr[0]);
    for (index = 1 ; index < size ; index++)
        FPRINTF(fp, ", %d", farr[index])
    FPRINTF(fp, " ];\n");
    FPRINTF(fp, "var toT   = [ %d", tarr[0]);
    for (index = 1 ; index < size ; index++)
        FPRINTF(fp, ", %d", tarr[index])
    FPRINTF(fp, " ];\n");
    FPRINTF(fp, "// End of Solution\n");
    return 1;
}

/**
 *    compute factorial value by the iterative way.
 *
 *    Inputs :
 *        int number  : The number which you want to find its factorial value.
 */
int iterativeFactorial(int number) {
    int index, total = 1;
    // using repeat function
    for (index = 1; index <= number; index++) {
        total *= index;
    }
    return total;
}

/**
 *    compute factorial value by the recursive way.
 *
 *    Inputs :
 *        int number  : The number which you want to find its factorial value.
 */
int recursiveFactorial(int number) {
    int total = 0;
    if ((number == 0) || (number == 1)) {
        return 1;
    } else {
    // "total = n * f(n-1)", which is recursion formula
        total = number * recursiveFactorial(number - 1);
        return total;
    }
}

/**
 *    print reversed string by the recursive way.
 *
 *    Inputs :
 *        char *str  : The string which you want to find its reversed string.
 */
void reverse(char *str) {
    // initial input value is the pointer of character array,
    // which is same as "str[0]"
    if (*str != 0) {
    // recurrence function
        reverse(str + 1);
    // when function find the "null" value, print character
    // which is the first of the string
    // print the first character of strings which are
    // from the last input string to the first input string.
        printf("%c", *str);
    }
}

/**
 *    Solve hanoi problem and output some value.
 *
 *    Inputs :
 *        int ndisks  : Number of disks which the user inputs.
 *        int from    : This is exactly the first stick.
 *        int to      : This is exactly the third stick.
 *        int via     : This is exactly the second stick.
 *        int size    : The number of movement of disks.
 *
 *    Outputs :
 *        int *farr : save integer value of "from"
 *        int *tarr : save integer value of "to"
 */
int hanoi(int ndisks, int from, int to,
          int via, int *farr, int *tarr, int size) {
    int attempts = 0;
    int tryNums = 0;
    if (ndisks == 1) {
    // record tower numbers into "from_arr" and "to_arr".
        while (1) {
            if ((*(farr + tryNums) <= 3) && (*(farr + tryNums) >= 1)) {
                tryNums += 1;
            } else {
                *(farr + tryNums) = from;
                *(tarr + tryNums) = to;
                break;
            }
        }
        return 1;
    } else {
    // execute 3 function at here:
    // 1. move "n-1" disks from "from" to "via".
    // 2. move 1 disk from "from" to "to".
    // 3. move "n-1" disks from "via" to "to".
    // counts attempts
        attempts += hanoi(ndisks - 1, from, via, to, farr, tarr, size);
    // record tower numbers into "from_arr" and "to_arr".
        while (1) {
            if ((*(farr + tryNums) <= 3) && (*(farr + tryNums) >= 1)) {
                tryNums += 1;
            } else {
                *(farr + tryNums) = from;
                *(tarr + tryNums) = to;
                break;
            }
        }
    // counts attempts
        attempts += 1;
    // counts attempts
        attempts += hanoi(ndisks - 1, via, to, from, farr, tarr, size);
        return attempts;
    }
}

