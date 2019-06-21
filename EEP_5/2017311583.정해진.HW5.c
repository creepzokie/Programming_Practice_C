/**
 * Copyright (c) 2003-2018 by Hyoung Bok Min, All rights reserved.
 * Copyright (c) 2018 by Hae Jin Jung, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * Programming Lab.5 : Structures
 *
 * Summary of this lab :
 * (1) When the program starts, first ask for the name of the data file.
 * (2) If the user gives the file name, the circuit data contained in the file
 *     is read. If there is a problem reading the data file, report the problem
 *     and exit the program. If there is no problem, go to (3).
 * (3) The program asks the command to process. When the user inputs
 *     the command and presses the Enter key, the command is processed
 *     and goes to step (3) again.
 *
 * Written by Jung, Hae Jin,
 *             School of Electrical and Electronics Engineering
 *             Sungkyunkwan University.
 *
 * File name : 2017311583.JungHaeJin.HW5.c
 *
 * Written on September 01, 2011
 * Modification History :
 *    1. Wrtten by Hyoung Bok Min on August 19, 2012 (given as a template)
 *    2. modified by Hae Jin Jung on May 7, 2018
 *       (a) Add codes for reading and writing text file (homeworks).
 *
 * This is the main program provided to students who registered
 * in "EE Programming Lab" at School of Electrical Engineering,
 * Sungkyunkwan University.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "circuit.h"
#define  _MEMCHECK_H_SIZE_OVER 500000
#include "memcheck.h"


int main(void) {
    void getFileName(char *filename, int bufsize);
    const int BUFSIZE = 1024;
    CMD_TYPE  command_type;
    COMPONENT *components;   // structure array of components
    int num_components;      // number of components at the above array
    const char *copy = "ckt, Copyright (c) 2003-2018 by Hyoung Bok Min";

    // Prepare buffer for file name and command
    char *filename = (char *)mymalloc(BUFSIZE*sizeof(char));
    assert(filename);
    char *command = filename;    // These 2 share memory.
    char *comp_name = (char *)mymalloc(BUFSIZE*sizeof(char));
    assert(comp_name);

#ifdef _WIN32
    // Copyright notice at Windows title bar
    const char *title_cmd = "title";
    assert((strlen(copy)+strlen(title_cmd)+1) < BUFSIZE);
    sprintf(command, "%s %s", title_cmd, copy);
    system(command);
#else
    printf("%s\n", copy);
#endif

    // Get name of input data file.
    getFileName(filename, BUFSIZE);

    // Read data on circuit information from the input data file, and
    // build structure array from the circuit information.
    num_components = readDataFile(filename, &components);
    if (num_components <= 0) {
        myfree(command);
        myfree(comp_name);
        // WARNING : DO NOT DELETE THE FOLLOWING LINE.
        check_memory();
        return 1;
    }
    printf("Data for %d components have been read.\n", num_components);

    do {
        COMPONENT_TYPE comp_type;
        char pin_name;

        // Get a command from user.
        char *msg =
            "Enter command (exit|count rlcb|avg rlcb|"
            "list name +-|share: ";
        printf("%s", msg);
        if (!fgets(command, BUFSIZE, stdin)) {
            printf("\nEOF key is detected, exiting ...\n");
            break;
        }

        // Interpret the user command.
        command_type = getCommand(command, &comp_type, comp_name, &pin_name);

        // Take action for the user command.
        switch (command_type) {
            case EXIT :
                break;
            case COUNT :    // command, "count"
                printCount(components, num_components, comp_type);
                break;
            case AVERAGE :  // command, "avg"
                printAverage(components, num_components, comp_type);
                break;
            case LIST:      // command, "list"
                printList(components, num_components, comp_name, pin_name);
                break;
            case SHARE:     // command, "share"
                printShare(components, num_components);
                break;
            case INVALID:   // invalid command
                printf("Invalid command\n");
                break;
            case BLANK:     // no command
                break;
            default :       // something wrong
                printf("We're afraid that we cannot service.\n");
                printf("Please report this problem to customer service.\n");
                break;
        }
    } while (command_type != EXIT);

    /* dispose memory used to store circuit data */
    myfree(command);
    myfree(comp_name);
    freeMemoryOfComponents(components, num_components);

    // Epilog of this program : This is the last part.
    // WARNING : DO NOT MODIFY THE FOLLOWING LINES.
    check_memory();
    return 0;
}


/**
 * Get a file name from keyboard (stdin).
 *
 * Inputs :
 *        int bufsize : size of file name buffer
 * Outputs :
 *        char *filename : a string buffer to return file name given by user.
 */
void getFileName(char *filename, int bufsize) {
    void trim(char *str);
    int  length;

    while (1) {
        printf("Enter file name : ");
        if (!fgets(filename, bufsize, stdin)) {
            printf("\nEOF key is not allowed.\n");
            continue;
        }
        length = strlen(filename);
        if ((filename[length-1] != '\n') && (filename[length-1] != '\r')) {
            printf("File name is too long.\n");
            continue;
        }
        trim(filename);
        if (filename[0]) {
            FILE *fp = myfopen(filename, "r");
            if (fp) {
                myfclose(fp);
                break;
            } else {
                printf("File \"%s\" does not exist.\n", filename);
            }
        } else {
            printf("You entered an empty file name.\n");
        }
    }
}


/**
 * Given a string, trim the string. In other words, remove both leading
 * and trailing white space characters.
 *
 * InOuts :
 *        char *str : a string given as an input,
 *                    leading and trailing white space chracters are removed,
 *                    and the trimed string becomes an output.
 *
 * NOTE: The string 'str' should be a mutable string.
 */
void trim(char *str) {
    char *p = str;
    int length = strlen(p);

    // rtrim
    while ((length > 0) && isspace(p[length-1]))
        p[--length] = 0;
    // ltrim
    while (*p && isspace(*p))
        ++p, --length;

    memmove(str, p, length+1);
}


/**
 *  Given a command ("exit", "count rlcb", "avg rlcb", etc),
 *  Get arguments of the command, and return the command type.
 *
 *  Inputs :
 *     char *command : a command which the user enter in a line.
 *
 *  Outputs :
 *     COMPONENT_TYPE *comp_type : component type,
 *                        this is valid only for "count" and "avg" commands.
 *     char *comp_name : component name,
 *                        this is the 1st argument of "list" command, and
 *                        memory space must be provided by calling function.
 *     char *pin_name :  pin of the above component,
 *                        this is the 2nd argument of list command, and
 *                        this is either '+' or '-'.
 *     Returns CMD_TYPE, which is one of INVALID, BLANK, EXIT, COUNT,
 *                        AVERAGE, LIST, and SHARE.
 */
CMD_TYPE getCommand(char *command, COMPONENT_TYPE *comp_type,
                    char *comp_name, char *pin_name) {
    CMD_TYPE cmd_type = INVALID;

    // Check whether command line is valid or not
    int length = strlen(command);
    if ((command[length-1] != '\n') && (command[length-1] != '\r')) {
        printf("Command line is too long\n");
        return cmd_type;
    }
    trim(command);

    // The command line is blank
    if (!(command[0]))
        return BLANK;

    // Prepare memory for instruction
    char *instruction = (char *)mymalloc((length+1)*sizeof(char));
    assert(instruction);

    // Extract only the command by removing arguments.
    if (sscanf(command, "%s", instruction) < 1) {
        myfree(instruction);
        return cmd_type;
    }

    // Is the command "exit" ?
    if (!strcmp(instruction, "exit")) {
        myfree(instruction);
        return EXIT;
    }

    // Process commands "count" and "avg".
    if (!strcmp(instruction, "count") || !strcmp(instruction, "avg")) {
        // Read a command argument which is a character.
        char ch_arg;
        int nread = sscanf(command, "%*s %c", &ch_arg);
        if (nread == 1) {
            if (!strcmp(instruction, "count"))
                cmd_type = COUNT;
            else
                cmd_type = AVERAGE;
            switch (ch_arg) {
                case 'r' :
                    *comp_type = RESISTOR;
                    break;
                case 'l' :
                    *comp_type = INDUCTOR;
                    break;
                case 'c' :
                    *comp_type = CAPACITOR;
                    break;
                case 'b' :
                    *comp_type = BATTERY;
                    break;
                default :
                    cmd_type = INVALID;
                    printf("The argument must be one of r,l,c, or b, ");
                    break;
            }
        } else {
            printf("Need an argument which is one of r,l,c, or b, ");
        }
    } else if (!strcmp(instruction, "list")) {   // Process "list" command.
        // Read command arguments "name" and "pin_name".
        int nread = sscanf(command, "%*s%s %c", comp_name, pin_name);
        if (nread == 2) {
            if (*pin_name != '+' && *pin_name != '-') {
                printf("Pin name must be either + or -, ");
            } else {
                cmd_type = LIST;
            }
        } else {
            printf("Need two arguments, ");
            printf("component-name and pin-name (+ or -), ");
        }
    } else if (!strcmp(instruction, "share")) {   // Process "share" command.
        // Check arguments.
        int nread = sscanf(command, "%*s%s", comp_name);
        if (nread != 1) {
            cmd_type = SHARE;
        } else {
            printf("Need no argument, ");
        }
    }

    myfree(instruction);
    return cmd_type;
}

int readDataFile(char *filename, COMPONENT **p_components) {
    // open input file by text reading mode
    FILE *fpi;
    fpi = myfopen(filename, "r");
    // setting variables of saving data of binary file.
    int rdNumComps;
    const int BUFSIZE = 1024;
    char compTYPE[1];     // saving Nth component's type
    double value;         // saving Nth component's value
    char *readLineBUF;
    char *readCNAME;
    char *readP;
    char *readM;
    // allocate the memory for "readLineBUF"
    readLineBUF = (char *)mymalloc(BUFSIZE * sizeof(char));
    assert(readLineBUF);
    // start to read first line of text file to detect the number of components
    fgets(readLineBUF, BUFSIZE, fpi);
    // read the number of components
    if (sscanf(readLineBUF, "%d ", &rdNumComps) == 1) {
        if (rdNumComps <= 0) {
            myfclose(fpi);
            fprintf(stderr, "[ERROR] '%s' ", filename);
            fprintf(stderr, "has the invalid number of components ");
            fprintf(stderr, "at the 1st line.\n");
            myfree(readLineBUF);
            return 0;
        }
    } else {
    // if reading fails, output the error message.
        myfclose(fpi);
        fprintf(stderr, "[ERROR] '%s' ", filename);
        fprintf(stderr, "doesn't have the number of components ");
        fprintf(stderr, "at the 1st line.\n");
        myfree(readLineBUF);
        return 0;
    }
    // count lines and check error about the invalid number of lines
    int countLines = 0;  // the variable of counting lines
    while (rdNumComps - countLines) {
    // print error message if file has less lines than the number of components
        if (!fgets(readLineBUF, BUFSIZE, fpi)) {
            myfclose(fpi);
            fprintf(stderr, "[ERROR] '%s' ", filename);
            fprintf(stderr, "has less data lines (%d) ", countLines);
            fprintf(stderr, "than the number ");
            fprintf(stderr, "specified at line 1 (%d).\n", rdNumComps);
            myfree(readLineBUF);
            return 0;
        }
    // print error message if file has blank line
        if (readLineBUF[0] == '\n') {
            myfclose(fpi);
            fprintf(stderr, "[ERROR] '%s' ", filename);
            fprintf(stderr, "has blank line ");
            fprintf(stderr, "at line %d.\n", countLines + 2);
            myfree(readLineBUF);
            return 0;
        }
        countLines++;
    }
    // print error message if file has more lines than the number of components
    if (fgets(readLineBUF, BUFSIZE, fpi)) {
        myfclose(fpi);
        fprintf(stderr, "[ERROR] '%s' ", filename);
        fprintf(stderr, "has more data lines than the number ");
        fprintf(stderr, "specified at line 1 (%d).\n", rdNumComps);
        myfree(readLineBUF);
        return 0;
    }
    fseek(fpi, 0, SEEK_SET);           // rewind the pointer of file
    fgets(readLineBUF, BUFSIZE, fpi);  // skip the first line of file
    // allocate the memory for "components" array
    COMPONENT *components = (COMPONENT *)mymalloc(rdNumComps *
                                                  sizeof(COMPONENT));
    // set reading line index
    int index = 0;
    // set error type variable because I want to free memory easily
    int errorTYPE = 0;
    // read lines in "infile.txt" until going to last line
    while (fgets(readLineBUF, BUFSIZE, fpi)) {
    // allocate the memory for temporary saving data array
        readCNAME = (char *)mymalloc(BUFSIZE * sizeof(char));
        assert(readCNAME);

        readP = (char *)mymalloc(BUFSIZE * sizeof(char));
        assert(readP);

        readM = (char *)mymalloc(BUFSIZE * sizeof(char));
        assert(readM);
    // check buffer overflow.
    // return value 0 if reading process do not execute successfully
        if (!strncmp(&readLineBUF[BUFSIZE], "\0", 1)) {
            myfclose(fpi);
            fprintf(stderr, "[ERROR] Overflow read line.\n");
            myfree(readLineBUF);
            myfree(readCNAME);
            myfree(readP);
            myfree(readM);
            if (index == 0) {
    // if first line is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another line is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        }
    // start reading data of each components
    // read Nth component's name
        if (sscanf(readLineBUF, "%s ", readCNAME) == 1) {
            if (strlen(readCNAME) <= 1) {
    // if component name does not exist, output the error message.
                myfclose(fpi);
                fprintf(stderr, "[ERROR] '%s' doesn't have the ", filename);
                fprintf(stderr, "%dth component's name ", index + 1);
                fprintf(stderr, "at line %d.\n", index + 2);
                myfree(readLineBUF);
                myfree(readCNAME);
                myfree(readP);
                myfree(readM);
                if (index == 0) {
                    errorTYPE = 1;
                    break;
                } else {
                    errorTYPE = 2;
                    break;
                }
            }
    // allocate memory of saving component name in "components" array
            components[index].name = (char *)mymalloc((strlen(readCNAME) + 1) *
                                                          sizeof(char));
            assert(components[index].name);
    // save component's name
            strncpy(components[index].name,
                    readCNAME,
                    (strlen(readCNAME) + 1));
    // free memory of temporary saving component's name
            myfree(readCNAME);
        }
    // read Nth component's type
        compTYPE[0] = (char)0;  // initialize variable saving type value
        if (sscanf(readLineBUF, "%*s %c", &compTYPE[0]) == 1) {
            if (compTYPE[0] == 'R') {
                components[index].type = RESISTOR;
            } else if (compTYPE[0] == 'L') {
                components[index].type = INDUCTOR;
            } else if (compTYPE[0] == 'C') {
                components[index].type = CAPACITOR;
            } else if (compTYPE[0] == 'B') {
                components[index].type = BATTERY;
            } else {
    // if component type is invalid, output the error message.
                myfclose(fpi);
                fprintf(stderr, "[ERROR] The type of ");
                fprintf(stderr, "%dth component ", index + 1);
                fprintf(stderr, "is invalid at line %d.\n", index + 2);
                myfree(readLineBUF);
                myfree(readP);
                myfree(readM);
                myfree(components[index].name);
                if (index == 0) {
    // if first line is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another line is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            }
        } else {
    // if component type does not exist, output the error message.
            myfclose(fpi);
            fprintf(stderr, "[ERROR] '%s' may not have the ", filename);
            fprintf(stderr, "%dth component's type ", index + 1);
            fprintf(stderr, "at line %d.\n", index + 2);
            myfree(readLineBUF);
            myfree(readP);
            myfree(readM);
            myfree(components[index].name);
            if (index == 0) {
    // if first line is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another line is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        }
    // read Nth component's value
        value = 0.0;
        if (sscanf(readLineBUF, "%*s %*c%lf", &value) == 1) {
            if (value < 0.0) {
                myfclose(fpi);
                fprintf(stderr, "[ERROR] %dth ", index + 1);
                fprintf(stderr, "component's value is invalid ");
                fprintf(stderr, "at line %d.\n", index + 2);
                myfree(readLineBUF);
                myfree(readP);
                myfree(readM);
                myfree(components[index].name);
                if (index == 0) {
    // if first line is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another line is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            }
            components[index].value = value;
        } else {
    // if reading fails, output the error message.
            myfclose(fpi);
            fprintf(stderr, "[ERROR] '%s' doesn't have no or ", filename);
            fprintf(stderr, "invalid the %dth component's value ", index + 1);
            fprintf(stderr, "at line %d.\n", index + 2);
            myfree(readLineBUF);
            myfree(readP);
            myfree(readM);
            myfree(components[index].name);
            if (index == 0) {
    // if first line is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another line is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        }
    // start reading data of components
    // which are linked with the Nth component with '+' sign
    // read a number of the components
        if (sscanf(readLineBUF, "%*s %*c%*f %s ", readP) == 1) {
            if (readP[0] != 'n') {
                myfclose(fpi);
                fprintf(stderr, "[ERROR] ");
                fprintf(stderr, "'%s' has invalid ", filename);
                fprintf(stderr, "the wire's name linked with the ");
                fprintf(stderr, "%dth component with plus sign ", index + 1);
                fprintf(stderr, "at line %d.\n", index + 2);
                myfree(readLineBUF);
                myfree(readP);
                myfree(readM);
                myfree(components[index].name);
                if (index == 0) {
    // if first line is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another line is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            }
    // allocate memory of saving component name
    // which is linked with plus sign in "components" array
            components[index].plus = (char *)mymalloc((strlen(readP) + 1) *
                                                      sizeof(char));
            assert(components[index].plus);
    // save the component's name
            strncpy(components[index].plus, readP, (strlen(readP) + 1));
    // free memory of temporary saving the component's name
            myfree(readP);
        } else {
    // if reading fails, output the error message.
            myfclose(fpi);
            fprintf(stderr, "[ERROR] ");
            fprintf(stderr, "'%s' doesn't have ", filename);
            fprintf(stderr, "the wire's name linked with the ");
            fprintf(stderr, "%dth component with plus sign ", index + 1);
            fprintf(stderr, "at line %d.\n", index + 2);
            myfree(readLineBUF);
            myfree(readP);
            myfree(readM);
            myfree(components[index].name);
            myfree(components[index].plus);
            if (index == 0) {
    // if first line is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another line is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        }
    // start reading data of components
    // which are linked with the Nth component with '-' sign
    // read a number of the components
        if (sscanf(readLineBUF, "%*s %*c%*f %*s %s", readM) == 1) {
            if (readM[0] != 'n') {
                myfclose(fpi);
                fprintf(stderr, "[ERROR] ");
                fprintf(stderr, "'%s' has invalid ", filename);
                fprintf(stderr, "the wire's name linked with the ");
                fprintf(stderr, "%dth component with minus sign ", index + 1);
                fprintf(stderr, "at line %d.\n", index + 2);
                myfree(readLineBUF);
                myfree(readM);
                myfree(components[index].name);
                myfree(components[index].plus);
                if (index == 0) {
    // if first line is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another line is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            }
    // allocate memory of saving component name
    // which is linked with minus sign in "components" array
            components[index].minus = (char *)mymalloc((strlen(readM) + 1) *
                                                       sizeof(char));
            assert(components[index].minus);
    // save the component's name
            strncpy(components[index].minus, readM, (strlen(readM) + 1));
    // free memory of temporary saving the component's name
            myfree(readM);
        } else {
    // if reading fails, output the error message.
            myfclose(fpi);
            fprintf(stderr, "[ERROR] ");
            fprintf(stderr, "'%s' doesn't have ", filename);
            fprintf(stderr, "the wire's name linked with the ");
            fprintf(stderr, "%dth component with minus sign ", index + 1);
            fprintf(stderr, "at line %d.\n", index + 2);
            myfree(readLineBUF);
            myfree(readM);
            myfree(components[index].name);
            myfree(components[index].plus);
            myfree(components[index].minus);
            if (index == 0) {
    // if first line is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another line is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        }
        index++;  // count line number if reading line is successful
    }
    if (errorTYPE == 1) {
    // if error type is 1, nothing to free memory of data read successfully
        myfree(components);
        return 0;
    } else if (errorTYPE == 2) {
    // if error type is 2, free memory of data read successfully
        for (int i = 0; i < index; i++) {
            myfree(components[i].name);
            myfree(components[i].plus);
            myfree(components[i].minus);
        }
        myfree(components);
        return 0;
    } else {
        // close input file
        myfclose(fpi);
        // points "p_components" to "components" for saving value globally
        *p_components = components;
        // free memory allocated line buffer
        myfree(readLineBUF);
        // return a number of components
        return rdNumComps;
    }
}

void printCount(COMPONENT *components,
                int num_components,
                COMPONENT_TYPE comp_type) {
    // set variable of counting the number of particular type
    int countCOMP_TYPE = 0;
    // count the number of particular type
    for (int index = 0; index < num_components; index++) {
        if (components[index].type == comp_type) {
            countCOMP_TYPE++;
        }
    }
    printf("There are %d components.\n", countCOMP_TYPE);
}

void printAverage(COMPONENT *components,
                  int num_components,
                  COMPONENT_TYPE comp_type) {
    // set variable of sum of value of components
    double sumVALUE = 0.0;
    // set variable of counting the number of particular type
    int countINDEX = 0;
    for (int index = 0; index < num_components; index++) {
        if (components[index].type == comp_type) {
            sumVALUE += components[index].value;
            countINDEX++;
        }
    }
    printf("Average value of %d components ", countINDEX);
    // print the average of sum of value of components
    printf("is %.3f.\n", sumVALUE / (double)countINDEX);
}

void printList(COMPONENT *components,
               int num_components,
               char *comp_name,
               char pin_name) {
    int stdINDEX = 0;    // standard index for comparing
    int countINDEX = 0;  // count the number of particular type components
    int commaINDEX = 0;  // count the number of comma to print
    // set standard index
    for (int index = 0; index < num_components; index++) {
        if (strcmp(components[index].name, comp_name) == 0) {
            stdINDEX = index;
            break;
        }
    }
    // if std input pin_name is plus, execute below
    if (pin_name == '+') {
    // count the number of particular type components
        for (int index = 0; index < num_components; index++) {
            if (strcmp(components[index].name, comp_name) != 0) {
                if (strcmp(components[index].plus,
                           components[stdINDEX].plus) == 0) {
                    countINDEX++;
                }
                if (strcmp(components[index].minus,
                           components[stdINDEX].plus) == 0) {
                    countINDEX++;
                }
            }
        }
        for (int index = 0; index < num_components; index++) {
            if (strcmp(components[index].name, comp_name) != 0) {
                if (strcmp(components[index].plus,
                           components[stdINDEX].plus) == 0) {
                    printf("+");
                    printf("%s", components[index].name);
                    commaINDEX++;
                    if ((commaINDEX >= 1) && (commaINDEX < countINDEX)) {
                        printf(", ");
                    }
                }
                if (strcmp(components[index].minus,
                           components[stdINDEX].plus) == 0) {
                    printf("-");
                    printf("%s", components[index].name);
                    commaINDEX++;
                    if ((commaINDEX >= 1) && (commaINDEX < countINDEX)) {
                        printf(", ");
                    }
                }
            }
        }
        printf("\n");
        printf("%d components ", countINDEX);
        printf("are connected to pin + of component ");
        printf("'%s'.\n", comp_name);
    }
    // if std input pin_name is minus, execute below
    if (pin_name == '-') {
    // count the number of particular type components
        for (int index = 0; index < num_components; index++) {
            if (strcmp(components[index].name, comp_name) != 0) {
                if (strcmp(components[index].plus,
                           components[stdINDEX].minus) == 0) {
                    countINDEX++;
                }
                if (strcmp(components[index].minus,
                           components[stdINDEX].minus) == 0) {
                    countINDEX++;
                }
            }
        }
        for (int index = 0; index < num_components; index++) {
            if (strcmp(components[index].name, comp_name) != 0) {
                if (strcmp(components[index].plus,
                           components[stdINDEX].minus) == 0) {
                    printf("+");
                    printf("%s", components[index].name);
                    commaINDEX++;
                    if ((commaINDEX >= 1) && (commaINDEX < countINDEX)) {
                        printf(", ");
                    }
                }
                if (strcmp(components[index].minus,
                           components[stdINDEX].minus) == 0) {
                    printf("-");
                    printf("%s", components[index].name);
                    commaINDEX++;
                    if ((commaINDEX >= 1) && (commaINDEX < countINDEX)) {
                        printf(", ");
                    }
                }
            }
        }
        printf("\n");
        printf("%d components ", countINDEX);
        printf("are connected to pin - of component ");
        printf("'%s'.\n", comp_name);
    }
}

void printShare(COMPONENT *components, int num_components) {
    int countSHARE = 0;  // count components sharing nodes
    for (int index1 = 0; index1 < num_components; index1++) {
        for (int index2 = index1 + 1; index2 < num_components; index2++) {
            if (index1 != index2) {
                if ((strcmp(components[index1].plus,
                            components[index2].plus)
                    || strcmp(components[index1].minus,
                              components[index2].minus)) == 0) {
                    printf("%s, ", components[index1].name);
                    printf("%s\n", components[index2].name);
                    countSHARE++;
                }
                if ((strcmp(components[index1].plus,
                            components[index2].minus)
                    || strcmp(components[index1].minus,
                              components[index2].plus)) == 0) {
                    printf("%s, ", components[index1].name);
                    printf("%s\n", components[index2].name);
                    countSHARE++;
                }
            }
        }
    }
    printf("%d component groups share wires.\n", countSHARE);
}

void freeMemoryOfComponents(COMPONENT *components, int num_components) {
    // free memory of "components" array
    for (int index = 0; index < num_components; index++) {
        myfree(components[index].name);
        myfree(components[index].plus);
        myfree(components[index].minus);
    }
    myfree(components);
}

/*------------- ckt.c -----------------*/

