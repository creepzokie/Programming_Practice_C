/*
 *    Copyright (c) 2012-2018 by Hyoung Bok Min, All rights reserved.
 *    For copyright and license information, please visit
 *    http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *    Data structure for Structures and Unions
 *
 *    PLEASE NEVER MODIFY THIS FILE.
 */

typedef enum {MALE, FEMALE} GENDER;   // gender type

typedef union _info {	// gender dependent information
    unsigned phone_no;  // 8-digit phone number, only for male
    char* city;         // city where she lives, only for female
} INFO;

typedef struct _person {
    char   *name;            // name
    short  age;              // age
    double height;           // height in feet
    GENDER gender;           // either MALE or FEMALE
    struct _person *spouse;  // spouse of this person
    INFO   info;             // gender-specific information
} PERSON;


