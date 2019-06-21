/**
 * Copyright (c) 2003-2017 by Hyoung Bok Min, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * Programming Lab. : Handling Binary Files
 *
 * This is the main program provided to students who registered
 * in "Programming Lab" at School of Information and Communication,
 * Sungkyunkwan University.
 *
 *
 * ===========================================================================
 * +++++++++++++++++++++
 * +++++  WARNING  +++++
 * +++++++++++++++++++++
 *
 * (1) IF YOU USE TYPES WHOSE BIT SIZE's ARE IMPORTANT,
 *     YOU HAVE TO USE THE TYPES SUCH AS BYTE, WORD, DWORD, etc IN datatypes.h.
 *     IF YOU USE char, short, int, etc FOR BIT-SPECIFIC TYPES,
 *     GRADE OF YOUR PROGRAM STYLE SECTION WILL BE 0.
 *
 * (2) DO NOT MODIFY STATEMENTS OF main().
 *     You can add or delete comments in main(), but, you cannot add or
 *     delete statements in main().
 * ===========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "datatypes.h"


int main(void) {
    // functions used by main()
    int getFileName(char *lnkfile, int size);
    int getInteger(char *title, int min, int max);
    int getLink(char *lnkfile, int *isFile, char *pathName, int sizePathName);

    // local variables
    const int NAMESIZE = 1024;  // size of .lnk file name buffer
    char *lnkfile;      // .lnk file, i.e., short-cut file name
    char *pathName;     // File name that the shoft-cut points to
    int  isFile;        // 1 if file, 0 if folder
    int  sizePathName;  // size of pathName buffer

    // Allocate memory to .lnk file name buffer
    lnkfile = (char *)malloc(NAMESIZE*sizeof(char));
    assert(lnkfile);

    // Get input Windows shortcut file name
    if (!getFileName(lnkfile, NAMESIZE)) {
        free(lnkfile);
        return 1;
    }
    printf("    entered name of Short-cut file as '%s'.\n", lnkfile);

    /**
     * Get size of target path name buffer.
     * This is to test whther students give proper error message
     *     at function getLink() when this size is too small.
     */
    sizePathName = getInteger("size of path name buffer", 1, NAMESIZE);
    printf("    entered path name buffer size as '%d'.\n", sizePathName);

    /**
     * Prepare memory to hold path name which is read from shortcut file.
     */
    pathName = (char *)malloc(sizePathName*sizeof(char));
    assert(pathName);

    /**
     * Get target file name and information.
     * You have to give proper error message if sizePathName is too small.
     * Also you have to give proper error messages for the other problems, too.
     */
    if (!getLink(lnkfile, &isFile, pathName, sizePathName)) {
        free(lnkfile);
        free(pathName);
        return 2;
    }

    // Output the results to stdout
    if (isFile)
        printf("The file '%s' is a short-cut to a FILE ", lnkfile);
    else
        printf("The file '%s' is a short-cut to a FOLDER ", lnkfile);
    printf("'%s'\n", pathName);
    free(lnkfile);
    free(pathName);
    return 0;
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
 * NOTE: The parameter 'str' should be a mutable string.
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
 * Get input Windows shortcut file name
 * String buffer to hold the name should be given by caller.
 *
 * Outputs :
 *         char *lnkfile  : buffer which hold name of Windows shortcut file
 *
 * Inputs :
 *         int  size : size of the above lnkfile buffer
 *
 * Returns 1 if this function gives valid file name,
 *         0 if file lnkfile is invalid.
 */
int getFileName(char *lnkfile, int size) {
    while (1) {
        FILE *fp;
        int  len;

        // Get .lnk file name from stdin.
        printf("Enter name of Short-Cut file (.lnk) : ");
        if (!fgets(lnkfile, size, stdin)) {
            char *msg =
                "\nUnexpected error during file name input. "
                "Please try again.\n";
            printf("%s", msg);
            continue;
        }

        // Check whether the file name is too long.
        // If there is no end-of-line character, it means that user entered
        //     very long string.
        len = strlen(lnkfile);
        if ((lnkfile[len-1] != '\n') && (lnkfile[len-1] != '\r')) {
            printf("[ERROR] File name is too long.\n");
            printf("        The name length should be %d or less.\n", size-2);
            return 0;
        }

        // remove both leading and trailing white space characters.
        trim(lnkfile);
        if (!(lnkfile[0])) {   // user hit only return key
            printf("File name is required.\n");
            continue;
        }

        // check whether the file exists or not
        fp = fopen(lnkfile, "rb");
        if (fp) {
            fclose(fp);
            break;
        }
        printf("File '%s' not found.\n", lnkfile);
    }
    return 1;
}


/**
 * Get an integer from keyboard
 *
 * Inputs :
 *         char *title : a string which is shown to user on screen
 *         int  min    : minimum intger from keyboard
 *         int  max    : maximum intger from keyboard
 *
 * Returns the integer from keyboard.
 * If user simply hit enter key, max value is returned.
 */
int getInteger(char *title, int min, int max) {
    const int BUFFER_SIZE = 128;  // size of the following buffer
    char *buffer;                 // buffer to hold number from keyboard
    char *garbage;                // garbage buffer for invalid user input
    int  the_integer = max;       // integer number given by user

    buffer = (char *)malloc(BUFFER_SIZE*sizeof(char));
    garbage = (char *)malloc(BUFFER_SIZE*sizeof(char));
    assert(buffer && garbage);

    while (1) {
        int  size, len, nitem;

        // Get an integer string from stdin.
        printf("Enter %s (min = %d, max = %d) : ", title, min, max);
        if (!fgets(buffer, BUFFER_SIZE, stdin)) {
            printf("\nUnexpected error during user input. ");
            printf("Please try again.\n");
            continue;
        }

        // Check user input is too long.
        // If there is no end-of-line character, it means that user entered
        //     very long string.
        len = strlen(buffer);
        if ((buffer[len-1] != '\n') && (buffer[len-1] != '\r')) {
            char *msg =
                "[ERROR] Your input is too long.\n"
                "        The input length should be %d or less.\n";
            printf(msg, BUFFER_SIZE-2);
            the_integer = 0;
            break;
        }

        nitem = sscanf(buffer, "%d%s", &size, garbage);
        if (nitem == 2) {  // user give another input in addition to a number
            printf("Please enter only one integer, and no more.\n");
        } else if (nitem < 0) {    // user hit only the return key
            // the_integer is max which is default value.
            break;
        } else if (nitem == 0) {   // user gives input which is not a number
            trim(buffer);
            printf("%s '%s' is not an integer.\n", title, buffer);
        } else if (size < min) {
            trim(buffer);
            printf("%s '%s' is too small.\n", title, buffer);
            printf("It should be larger than or equal to %d.\n", min);
        } else if (size > max) {
            trim(buffer);
            printf("%s '%s' is too large.\n", title, buffer);
            printf("It should be less than or equal to %d.\n", max);
        } else {
            the_integer = size;
            break;
        }
    }
    free(buffer);
    free(garbage);
    return the_integer;
}


// WARNING : If you do not use functions inside getLink() properly,
//           your loss of grades may be significant.
int getLink(
    char *lnkfile,     // input  : shortcut file name
    int  *isFile,      // output : 1 if file, 0 if folder
    char *pathName,    // output : string buffer to hold the link path name
    int  sizePathName  // input  : size of the above string buffer
    ) {

    /* YOUR PROGRAM CODE HERE */

}
/*-------------------  shortcut.c -------------------------*/
