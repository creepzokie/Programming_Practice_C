/**
 *  Copyright (c) 2016-2018 by Hyoung Bok Min, All rights reserved.
 *  For license information, please refer to
 *      http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *  Programming Lab. : Array of Strings
 *
 *  File name : grade.h
 *
 *  Purpose   : Helper functions to grade students' program codes.
 *              This file gives function prototypes of those helper functions,
 *              and the function bodies are implemented in file "grade.c".
 *
 *  Note      : This file should be included in file "string.c".
 */

// Check pre-conditions for grading.
extern int  grade_init(int num_persons, int num_grades);

// Grade function "readTXT()" coded by students.
extern void grade_txt_array(char *input_file, int num_persons, int num_grades,
                  char **name_array, char *gender_array, int **grades_array);

// Grade function "writeBIN()" coded by students.
extern void grade_bin_file(char *binary_file, int num_persons, int num_grades,
                  char **name_array, char *gender_array, int **grades_array);

// Grade function "readBIN()" coded by students.
extern void grade_read_bin(char *binary_file, int num_persons, int num_grades,
                  char **name_array, char *gender_array,
                  int **grades_array, double *average_array);

// Grade function "writeTXT()" coded by students.
extern void grade_txt_file(char *output_file, int num_persons, int num_grades,
                  char **name_array, char *gender_array,
                  int **grades_array, double *average_array);
/*--- grade.h ---*/
