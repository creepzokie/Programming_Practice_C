/**
 *    Copyright (c) 2004-2018 by Hyoung Bok Min, All rights reserved.
 *    Copyright (C) 2018 by Hae Jin Jung, All rights reserved.
 *    For license information, please refer to
 *        http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *    Programming Lab. Arrays (and Pointers)
 *
 *    This is the main program provided to students who registered
 *    in "Programming Lab" at School of Information and Communication,
 *    Sungkyunkwan University.
 *
 *    Delete comments which are not required.
 *    Add comments which are required.
 *
 *    WARNING : YOU CANNOT MODIFY FUNCTION main() EXCEPT THE AREA WITH
 *               COMMENTS SUCH AS "WRITE ... ".
 *              BUT, YOU CAN ADD malloc(), free() and FUNCTION PROTOTYPES
 *               IF REQUIRED.
 *
 *    WARNING : YOU HAVE TO DISCUSS THE TOPICS GIVEN AS "REPORT" BELOW.
 *
 *    WARNING : Return type of every function must be "void", i.e.,
 *              those function should return nothing.
 *              ONE EXCEPTION : function to compute average at [2]
 *                              should return double.
 *
 *    ADD functions so that this program works.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define  BUFSIZE 1024
#define  XSIZE   30

/**
 *    Main program of this programming lab.
 */
int main(void) {
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     *
     *               PART  1 : Array and pointer basics
     *
     *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    /*********************************************
     * [1] Pointers
     *********************************************/
    printf("+---------------------+\n");
    printf("|  Practice Pointers  |\n");
    printf("+---------------------+\n");
    /**
     * [1-1] Pointer Basics
     */
    {  // BEGIN_BLOCK
    int miles = 1200;
    int *ptr2miles;          // pointer to 'miles' variable
    int **ptr2ptr2miles;     // double pointer to 'miles' variable

    ptr2miles = &miles;
    ptr2ptr2miles = &ptr2miles;

    // We want to print the same 3 integers by the following code,
    //      using the variable, pointer, and double-pointer
    printf("## expecting the same 3 integers 1200.\n");
    printf("miles = %d %d %d\n", miles, *ptr2miles, **ptr2ptr2miles);
    }  // END_BLOCK

    /**
     * [1-2] Pointer for function output arguments
     *    (A) addition and subtraction of 2 integers
     */
    {  // BEGIN_BLOCK
    int  num1 = 20, num2 = 10;
    int  added;         // added result of "num1 + num2"
    int  subtracted;    // subtracted result of "num1 - num2"

    // function call to add and subtract in a function

    void addSubtract(int num1, int num2, int *added, int *subtracted) {
        *added = num1 + num2;
        *subtracted = num1 - num2;
    }

    addSubtract(num1, num2, &added, &subtracted);
    printf("## expecting two numbers and add, sub of the numbers.\n");
    printf("num1 = %d, num2 = %d\n", num1, num2);
    printf("added = %d, subtracted = %d\n", added, subtracted);

    /**
     *    (B) Swap 2 integers
     */

    /**
     *   Given two integers, do not swap each numbers.
     *
     *   Outputs:
     *
     *   Inputs:
     *       int num1 : the number which is 20
     *       int num2 : the number which is 10
     */

    void noSwap(int num1, int num2) {
        int switching_nums = num1;    // the variable saving the value of num1
        num1 = num2;                  // because of not using pointer, no swap
        num2 = switching_nums;
    }

    /**
     *   Given two integers, swap each numbers.
     *
     *   Outputs:
     *
     *   Inputs:
     *       int num1 : the number which is 20
     *       int num2 : the number which is 10
     */

    void doSwap(int *num1, int *num2) {
        int switching_nums = *num1;    // the variable saving the value of num1
        *num1 = *num2;                 // because of using pointer, swap
        *num2 = switching_nums;
    }

    noSwap(num1, num2);    // cannot swap
    printf("## expecting no-swap, i.e. 20, 10.\n");
    printf("after noSwap : num1 = %d, num2 = %d\n", num1, num2);

    doSwap(&num1, &num2);  // perform swap
    printf("## expecting do-swap, i.e. 10, 20.\n");
    printf("after doSwap : num1 = %d, num2 = %d\n", num1, num2);
    }  // END_BLOCK


    /****************************************
     * [2] Array
     ****************************************/
    printf("+-------------------+\n");
    printf("|  Practice Arrays  |\n");
    printf("+-------------------+\n");
    {  // BEGIN_BLOCK
    int  firstRandoms[10];
    printf("## processing the following 10 numbers in array:\n");
    for (int index = 0 ; index < 10 ; index++) {
        firstRandoms[index] = rand() % 100;
        printf(" %d", firstRandoms[index]);
    }
    printf("\n");

    /**
     * [2-1] Get average of the above random numbers
     *       Write your function call below, and print the averge.
     */

    /**
     *   Given a 1-dimensional array of integers filled with random numbers
     *   between 0 and 100, print average of numbers in the array.
     *
     *   Outputs:
     *       average_of_randnums : average of the random numbers in the array
     *
     *   Inputs:
     *       int *randnums_array : array of integers filled with random numbers
     *                             between 0 and 100
     */

    double aver_randnums(int *randnums_array) {
        int sum_of_randnums = 0;
        double average_of_randnums = 0.0;
        for (int index = 0 ; index < 10 ; index++) {
            sum_of_randnums += randnums_array[index];
    // the variable which is for calculating sum of random numbers
        }
        average_of_randnums = (double)sum_of_randnums / 10.0;
        return average_of_randnums;
    }

    double average = aver_randnums(firstRandoms);
    printf("average of the above 10 integers = %.2lf\n", average);
    }  // END_BLOCK

    /**
     * [2-2] Array as a pointer and dynamic memory allocation.
     */
    {  // BEGIN_BLOCK
    int *secondRandoms;   // array as a pointer

    /**
     *   Given a 1-dimensional array of integers filled with random numbers
     *   between 0 and 100, print average of numbers in the array.
     *
     *   Outputs:
     *       average_of_randnums : average of the random numbers in the array
     *
     *   Inputs:
     *       int *randnums_array : array of integers filled with random numbers
     *                             between 0 and 100
     */

    double aver_randnums(int *randnums_array) {
        int sum_of_randnums = 0;
        double average_of_randnums = 0.0;
        for (int index = 0 ; index < 10 ; index++) {
            sum_of_randnums += randnums_array[index];
    // the variable which is for calculating sum of random numbers
        }
        average_of_randnums = (double)sum_of_randnums / 10.0;
        return average_of_randnums;
    }

    /**
     *    (A) We cannot use the following expression at this time.
     *        Observe RUN TIME ERROR, and comment-out the followng
     *        1 line of code. We'll continue without the following line.
     *        REPORT : YOU HAVE TO EXPLAIN WHY THIS LINE MAKES AN ERROR
     *                 IN YOUR REPORT.
     */
    // secondRandoms[0] = rand();

    /**
     *    (B) Get memory to hold 10 integers from operating systems
     *        by dynamic memory allocation,
     *        and Compute average for the 10 inetgers.
     */
    secondRandoms = (int *)malloc(10*sizeof(int));
    assert(secondRandoms);

    printf("## processing the following 10 numbers by dynamic alloc:\n");
    for (int index = 0 ; index < 10 ; index++) {
        secondRandoms[index] = rand() % 100;
        printf(" %d", secondRandoms[index]);
    }
    printf("\n");

    double average = aver_randnums(secondRandoms);
    printf("average of the above 10 integers = %.2lf\n", average);
    free(secondRandoms);
    }  // END_BLOCK

    /**
     * [2-3] Array as a function argument
     *    (A) Input argument
     *        Write your function 'printArray()' to print the array data.
     */

    void printArray(int *myNumbers, int len) {
        int k;
        for (k = 0; k < len ; k++) {
            printf(" %d", myNumbers[k]);
        }
    }

    {  // BEGIN_BLOCK
    int  myNumbers[10] =  { 10, 20, 30, 40, 50 };
    printf("## printArray() prints 10 integers in array:\n");
    printArray(myNumbers, 10);
    printf("\n");
    }  // END_BLOCK

    /**
     *    (B) Output argument
     *        Write your function 'fillArray()' so that array
     *            'myNumbers' holds 10 integers, 0, 10, 20, 30, ..., 90.
     */

    void fillArray(int *myNumbers, int len) {
        int k;
        for (k = 0 ; k < len ; k++) {
            myNumbers[k] = k*10;
        }
    }

    {  // BEGIN_BLOCK
    int  myNumbers[10];

    fillArray(myNumbers, 10);

    printf("## fillArray() put 10 numbers {0, 10, 20, ...} into array :\n");
    printArray(myNumbers, 10);
    printf("\n");
    }  // END_BLOCK

    /**
     *    (C) Output argument
     *        Write your function 'fillArrayPointer()' so that array
     *            'myNumbers' holds 10 integers, 0, 100, 200, 300, ..., 900.
     */

    void fillArrayPointer(int *myNumbers, int len) {
        int k;
        for (k = 0 ; k < len ; k++) {
            myNumbers[k] = k*100;
        }
    }

    {  // BEGIN_BLOCK
    int *myNumbers = (int *)malloc(10*sizeof(int));
    assert(myNumbers);

    fillArrayPointer(myNumbers, 10);

    printf("## fillArrayPointer() put {0, 100, 200, ...} into array :\n");
    printArray(myNumbers, 10);
    printf("\n");
    free(myNumbers);
    }  // END_BLOCK

    /************************************
     * [3] Strings
     ************************************/
    printf("+--------------------+\n");
    printf("|  Practice Strings  |\n");
    printf("+--------------------+\n");
    /**
     * [3-1] String as a character array
     *    (A) Initialization of string (character array)
     */
    {  // BEGIN_BLOCK
    char myname[100] = "Min, Hyoung Bok";
    printf("## init array string by professor's name :\n");
    printf("Professor's name is '%s'\n", myname);

    /**
     *    (B) Set value of a string by your name.
     *        Now, we want the 'myname' holds your name, but,
     *        we cannot use the following code.
     *
     *        myname = "Hong, Kil Dong";
     *
     *        You have to use system function "strcpy()".
     *        REPORT : Explain why we cannot use the above code,
     *                 and must use strcpy() in your report.
     */
    strcpy(myname, "Jung, Hae Jin");
    printf("## init array string by strcpy() with your name :\n");
    printf("My name is '%s'\n", myname);
    }  // END_BLOCK

    /**
     * [3-2] String as a pointer
     *    (A) Initialization of string (characater array)
     */
    {  // BEGIN_BLOCK
    char *myname = "Min, Hyoung Bok";
    printf("## init pointer string with professor's name :\n");
    printf("Professor's name is '%s'\n", myname);

    /**
     *    (B) Set value of a string by using 2 methods.
     *        (Method 1) By simple assignment.
     *        (Method 2) By using "strcpy()".
     *
     *        REPORT : Explain why we can use the above 2 in your report.
     */
    // Method 1
    myname = "Jung, Hae Jin";
    printf("## set pointer string with your name by assignment :\n");
    printf("My name is '%s'\n", myname);

    // Method 2
    // YOU HAVE TO DO SOMETHING BEFORE CALLING strcpy().
    // What's that ? Write your code to do the work here.

    myname = (char*)malloc(12*sizeof(char));
    // before using strcpy(), memory allocation must be needed
    strcpy(myname, "Jo, Jun Hui");
    printf("## set pointer string with your best friend's name by strcpy :\n");
    printf("Name of my best friend is '%s'\n", myname);
    free(myname);
    }  // END_BLOCK

    /**
     * [3-3] String as a function argument
     *    (A) Input argument
     *        Write your function 'printName()' to print your name.
     */

    void printName(char *myname) {
        printf("%s\n", myname);
    }

    {  // BEGIN_BLOCK
    char *myname = "Jung, Hae Jin";
    printf("## printName() prints pointer string :\n");
    printName(myname);
    }  // END_BLOCK

    /**
     *    (B) Output argument : Use array
     *        Write your function 'fillNameArray()' so that the char array
     *            'friend1Name' holds your friend's name.
     */

    void fillNameArray(char friend1Name[]) {
        char save_friend1name[] = "Jo, Jun Hui";
        strcpy(friend1Name, save_friend1name);
    }

    {  // BEGIN_BLOCK
    char friend1Name[256];
    fillNameArray(friend1Name);
    printf("## fillNameArray() prints pointer string :\n");
    printf("Name of my first friend is '%s'\n", friend1Name);
    }  // END_BLOCK

    /**
     *    (C) Output argument : Use pointer
     *        Write your function 'fillNamePointer()' so that the
     *            char pointer 'friend2Name' holds your another friend's name.
     *
     */

    void fillNamePointer(char **friend2Name) {    // using double pointer
        *friend2Name = "Cheon, Yeong Jun";
    }

    {  // BEGIN_BLOCK
    char *friend2Name;
    fillNamePointer(&friend2Name);
    printf("## fillNamePinter() prints pointer string :\n");
    printf("Name of my second friend is '%s'\n", friend2Name);
    }  // END_BLOCK

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     *
     *               PART  2 : draw graphs of sine and cosine waves
     *
     *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /**
     *    (A) draw sine wave (0 ~ 90 degrees) by using fixed size Array
     */
    {
    void drawSineWave(char graph[][70], int num_x, int num_y);
    void drawGraphArray(char graph[][70], int num_x, int num_y);
    char graph[XSIZE][70];

    drawSineWave(graph, XSIZE, 70);     // compute sine function

    printf("\n\nNOW, DRAWING SINE WAVE\n\n");
    drawGraphArray(graph, XSIZE, 70);   // draw the function on screen
    }

    /**
     *    (B) draw cosine wave (0 ~ 90 degrees) by using variable sized Array
     */
    {
    void drawCosineWave(char **graph, int num_x, int num_y);
    void drawGraphPointer(char **graph, int num_x, int num_y);
    void disposeMemory(char **graph, int num_x);

    // prepare memory
    char **graph = (char **)malloc(XSIZE*sizeof(char *));
    assert(graph);
    for (int k = 0 ; k < XSIZE ; k++) {
        graph[k] = (char *)malloc(70*sizeof(int));
        assert(graph[k]);
    }

    drawCosineWave(graph, XSIZE, 70);      // compute cosine function

    printf("\n\nNOW, DRAWING COSINE WAVE\n\n");
    drawGraphPointer(graph, XSIZE, 70);    // draw to screen
    disposeMemory(graph, XSIZE);           // free memory used in your function
    }

    return 0;
}


/**
 *   Given a 2-dimensional array of characters and size of the array,
 *   compute and save graph of sine wave from 0 to 90 degrees to the array.
 *   This function does the followings:
 *       (1) compute sine function values from 0 to 90 degrees.
 *           The degrees are divided by "num_x_samples". For example,
 *           if num_x_samples is 31, 31 x-axis points are used for the 0 to
 *           90 degrees, which means that sine values are computed
 *           for every 3 degree, including both 0 and 90 degrees.
 *       (2) The computed values are saved into 2-dimensional array,
 *           "char graph[][70]" as 1's and 0's. Since the 2nd indices of
 *           that array is used for sine values, 70 means 1.0, which is
 *           maximum of the sine values. The shape of the array should be
 *           as follows.
 *           ------------------> y axis
 *           |  0000000000000000000000000000000000000000000
 *           |  1111000000000000000000000000000000000000000
 *     x axis|  1111111000000000000000000000000000000000000
 *           v  1111111111100000000000000000000000000000000
 *              1111111111111110000000000000000000000000000
 *              ...
 *              ...  (skip intermediate values)
 *              ...
 *              1111111111111111111111111111111111111111000
 *              1111111111111111111111111111111111111111100
 *              1111111111111111111111111111111111111111110
 *              1111111111111111111111111111111111111111111
 *              1111111111111111111111111111111111111111111
 *              1111111111111111111111111111111111111111111
 *
 *   Outputs:
 *       char graph[][70] : computed results are saved in this array as
 *                          1's and 0's.
 *
 *   Inputs:
 *       int num_x_samples : number of x-axis samples for 0 ~ 90 degrees
 *                           (size of 1st index of the array)
 *       int num_y_samples : number of y-axis samples for 0.0 ~ 1.0
 *                           (size of 2nd index of the array)
 */

void drawSineWave(char graph[][70], int num_x_samples, int num_y_samples) {
    double num_y_sinValue = 0.0;
    double degreeToRadian = 0.0;
    for (int i = 0; i < num_x_samples; i += 1) {
        degreeToRadian = 3.1415926535 * (double)i / 60.0;
        num_y_sinValue = (double)num_y_samples * sin(degreeToRadian);
        for (int j = 0; j <= num_y_samples ; j += 1) {
            strcpy(&graph[i][j], "0");
        }
        for (int j = 0; j <= (int)num_y_sinValue ; j += 1) {
            if (num_y_sinValue == 0.0) {
                break;
            }
            strncpy(&graph[i][j], "1", 1);
        }
    }
}

/**
 *   Given a 2-dimensional array of characters and size of the array,
 *   compute and save graph of cosine wave from 0 to 90 degrees to the array.
 *   This function does the followings:
 *       (1) compute cosine function values from 0 to 90 degrees.
 *           The degrees are divided by "num_x_samples". For example,
 *           if num_x_samples is 31, 31 x-axis points are used for the 0 to
 *           90 degrees, which means that cosine values are computed
 *           for every 3 degree, including both 0 and 90 degrees.
 *       (2) The computed values are saved into 2-dimensional array,
 *           "char graph[][]" as 1's and 0's. Since the 2nd indices of
 *           that array is used for sine values, 70 means 1.0, which is
 *           maximum of the sine values. The shape of the array should be
 *           as follows.
 *           ------------------> y axis
 *           |  1111111111111111111111111111111111111111111
 *           |  1111111111111111111111111111111111111111111
 *     x axis|  1111111111111111111111111111111111111111111
 *           v  1111111111111111111111111111111111111111110
 *              1111111111111111111111111111111111111111100
 *              1111111111111111111111111111111111111111000
 *              ...
 *              ...  (skip intermediate values)
 *              ...
 *              1111111111111110000000000000000000000000000
 *              1111111111100000000000000000000000000000000
 *              1111111000000000000000000000000000000000000
 *              1111000000000000000000000000000000000000000
 *              0000000000000000000000000000000000000000000
 *
 *   Outputs:
 *       char graph[][] : computed results are saved in this array as
 *                          1's and 0's.
 *
 *   Inputs:
 *       int num_x_samples : number of x-axis samples for 0 ~ 90 degrees
 *                           (size of 1st index of the array)
 *       int num_y_samples : number of y-axis samples for 0.0 ~ 1.0
 *                           (size of 2nd index of the array)
 */

void drawCosineWave(char **graph, int num_x_samples, int num_y_samples) {
    double num_y_cosValue = 0.0;
    double degreeToRadian = 0.0;
    for (int i = 0; i < num_x_samples; i += 1) {
        degreeToRadian = 3.1415926535 * (double)i / 60.0;
        num_y_cosValue = (double)num_y_samples * cos(degreeToRadian);
        for (int j = 0; j <= num_y_samples ; j += 1) {
            strcpy(&graph[i][j], "0");
        }
        for (int j = 0; j <= (int)num_y_cosValue ; j += 1) {
            if (num_y_cosValue == 0.0) {
                break;
            }
            strncpy(&graph[i][j], "1", 1);
        }
    }
}

/**
 *   Given a 2-dimensional array of characters filled with 1's and 0's
 *   and size of the array, print '+' to stdout for only 1's in the array.
 *
 *   Outputs:
 *       char graph[][70] : sine wave graph of contiuous 1's followed by 0's.
 *
 *   Inputs:
 *       int num_x_samples : number of x-axis samples for 0 ~ 90 degrees
 *                           (size of 1st index of the array)
 *       int num_y_samples : number of y-axis samples for 0.0 ~ 1.0
 *                           (size of 2nd index of the array)
 */

void drawGraphArray(char graph[][70], int num_x_samples, int num_y_samples) {
    int compare_number = 1;
    for (int i = 0; i < num_x_samples; i += 1) {
        for (int j = 0; j < num_y_samples; j += 1) {
            if (strcmp(&graph[i][j], "1") == 1) {
                strncpy(&graph[i][j], "+", 1);
                printf("%c", graph[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 *   Given a 2-dimensional array of characters filled with 1's and 0's
 *   and pointer of the array, print '+' to stdout for only 1's in the array.
 *
 *   Outputs:
 *       char graph[][] : cosine wave graph of contiuous 1's followed by 0's.
 *
 *   Inputs:
 *       int num_x_samples : number of x-axis samples for 0 ~ 90 degrees
 *                           (size of 1st index of the array)
 *       int num_y_samples : number of y-axis samples for 0.0 ~ 1.0
 *                           (size of 2nd index of the array)
 */

void drawGraphPointer(char **graph, int num_x_samples, int num_y_samples) {
    int compare_number = 1;
    for (int i = 0; i < num_x_samples; i += 1) {
        for (int j = 0; j < num_y_samples; j += 1) {
            if (strcmp(&graph[i][j], "1") == 1) {
                strncpy(&graph[i][j], "+", 1);
                printf("%c", **(graph + i));
            }
        }
        printf("\n");
    }
}

void disposeMemory(char **graph, int num_x_samples) {
    for (int k = 0; k < num_x_samples; k += 1) {
        free(graph[k]);
// free each y axis memory
    }
    free(graph);
}

/*-------  array.c  --------*/
