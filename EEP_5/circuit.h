/**
 * Copyright (c) 2014-2018 by Hyoung Bok Min, All rights reserved.
 * For license information,
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 * 
 * Programming Lab. Structures
 *
 * This is the main program provided to students who registered
 * in "EE Programming Lab" at School of Electrical Engineering,
 * Sungkyunkwan University.
 *
 *
 * This file contains data structures of this program, and 
 * function declarations.
 */


// Component types
typedef enum { RESISTOR, INDUCTOR, CAPACITOR, BATTERY } COMPONENT_TYPE;

/**
 *	information of a component
 */
typedef struct {
    char *name;           // component name
    COMPONENT_TYPE type;  // component type
    double value;         // value of this component
    char *plus;           // wire name connected to plus pin
    char *minus;          // wire name connected to minus pin
} COMPONENT;


/**
 *	code of a command
 */
typedef enum { INVALID, BLANK, EXIT, COUNT, AVERAGE, LIST, SHARE } CMD_TYPE;


/**
 *	Function declarations which is used in this program
 */
extern int readDataFile(char *, COMPONENT **);
extern CMD_TYPE getCommand(char *, COMPONENT_TYPE *, char *, char *);
extern void printAverage(COMPONENT*, int, COMPONENT_TYPE);
extern void printCount(COMPONENT*, int, COMPONENT_TYPE);
extern void printList(COMPONENT *, int, char *, char);
extern void printShare(COMPONENT *, int);
extern void freeMemoryOfComponents(COMPONENT *, int);


/*-----------  circuit.h  -------------*/
