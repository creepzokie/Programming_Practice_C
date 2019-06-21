/**
 *    Copyright (c) 2003-2018 by Hyoung Bok Min, All rights reserved.
 *    For license information, please refer to
 *        http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *    Programming Lab : Structures
 *
 *    This is the main program provided to students who registered
 *    in "EE Programming Lab" at School of Electrical Engineering,
 *    Sungkyunkwan University.
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

/*------------- ckt.c -----------------*/
