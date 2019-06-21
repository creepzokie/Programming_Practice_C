/**
 * Copyright (c) 2003-2017 by Hyoung Bok Min, All rights reserved.
 * Copyright (c) 2018 by Hae Jin Jung, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *  Lab 3 of C Programming : Handling Binary Files
 *
 *  Summary of this lab :
 *  1. Read the first DWORD and LinkCLSID
 *     to verify that this file is a shortcut file.
 *     (If it is not a shortcut file, give an error message and end program.)
 *  2. In the header, make sure that the file points to the folder or file.
 *  3. Check whether the LinkTargetIDList and LinkInfo exist in the header.
 *     (Skips if LinkTargetIDList exists
 *      and ends with error message if LinkInfo is not present)
 *  4. Find the location of LocalBasePath and CommonPathSuffix
 *     in the Header of the LinkInfo,
 *     read the two values, and output the whole link path.
 *
 *  Written by Jung, Hae Jin,
 *             School of Electrical and Electronics Engineering
 *             Sungkyunkwan University.
 *
 *  File name : 2017311583.JungHaeJin.HW3.c
 *
 *  Written on September 01, 2011
 *  Modification History :
 *     1. Wrtten by Hyoung Bok Min on August 19, 2012 (given as a template)
 *     2. modified by Hae Jin Jung on April 9, 2018
 *        (a) Add codes for reading and writing text file (homeworks).
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

/**
 *   Given integer pointer and character pointer,
 *   output the value of file's pointing position's type
 *   and the link path name.
 *
 *   Outputs:
 *       *isFile       : 1 if file, 0 if folder
 *       *pathName     : string buffer to hold the link path name
 *
 *   Inputs:
 *       *lnkfile      : shortcut file name
 *       sizePathName  : size of the above string buffer
 */

int getLink(
    char *lnkfile,     // input  : shortcut file name
    int  *isFile,      // output : 1 if file, 0 if folder
    char *pathName,    // output : string buffer to hold the link path name
    int  sizePathName  // input  : size of the above string buffer
    ) {
    FILE *fp;
    fp = fopen(lnkfile, "rb");
    if (!fp) {
        fprintf(stderr, "[ERROR] File '%s' not found.\n", lnkfile);
        fclose(fp);
        return 0;
    }
    // Read HeaderSize first.
    DWORD mayLnkF[1];  // The array for saving HeaderSize value.
    // Read HeaderSize value as DWORD size.
    fread(mayLnkF, sizeof(DWORD), 1, fp);
    if (*mayLnkF != 0x4c) {
    // HeaderSize value is not same as link file's headerSize value.
        fprintf(stderr, "[WARNING] ");
        fprintf(stderr, "'%s' may not be a short-cut file. ", lnkfile);
        fprintf(stderr, "Header size is '%#x' (expected 0x4c).\n", *mayLnkF);
        fprintf(stderr, "[WARNING] Output may be wrong ");
        fprintf(stderr, "since we do not support this file.\n");
    }

    // Read LinkCLSID.
    DWORD *isLnkF;
    // Dynamic memory allocation of array saving the LinkCLSID value
    isLnkF = (DWORD *)malloc(16);
    assert(isLnkF);
    // Read LinkCLSID value as 4*DWORD size.
    fread(isLnkF, 4*sizeof(DWORD), 1, fp);
    if ((isLnkF[0] != 0x21401) || (isLnkF[1] != 0)
        || (isLnkF[2] != 0xC0) || (isLnkF[3] != 0x46000000)) {
    // LinkCLSID value is not same as link file's LinkCLSID value.
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, "Invalid LinkCLSID of File '%s'.\n", lnkfile);
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, "This is not a Windows shortcut file.\n");
        fclose(fp);
        return 0;
    }
    free(isLnkF);       // Free the memory allocated for isLnkF

    // Read LinkFlags.
    DWORD linkFlag[1];  // The array for saving LinkFlags value.
    // Read LinkFlags value as DWORD size.
    fread(linkFlag, sizeof(DWORD), 1, fp);
    if (((*linkFlag << 30) >> 30) == 3) {
    // By using the shift operator,
    // we can see "hasLinkTargetIDList" value and "hasLinkInfo" value.
    // The file has LinkTargetIDList and LinkInfo.
    } else if (((*linkFlag << 30) >> 30) == 2) {
    // The file has LinkInfo but no LinkTargetIDList.
    } else if (((*linkFlag << 30) >> 30) == 1) {
    // The file has LinkTargetIDList but no LinkInfo.
    } else {
    // The file doesn't have both LinkTargetIDList and LinkInfo.
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, "Invalid LinkTargetIDList or LinkInfo ");
        fprintf(stderr, "of File '%s'.\n", lnkfile);
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, "This is not a Windows shortcut file.\n");
        fclose(fp);
        return 0;
    }

    // Read FileAttributes.
    DWORD FileAtbute[1];   // The array for saving FileAttributes value.
    // Read FileAttributes value as DWORD size.
    fread(FileAtbute, sizeof(DWORD), 1, fp);
    if (((*FileAtbute << 27) >> 31) == 1) {
    // By using the shift operator,
    // we can see "FILE_ATTRIBUTE_DIRECTORY" value.
    // It points to folder.
        *isFile = 0;
    } else {
    // It points to file.
        *isFile = 1;
    }

    // Read IDListSize which is component of LinkTargetIDList.
    WORD IDListSize[1];    // The array for saving IDListSize value.
    // We have to skip unnecessary value of HEADER
    // if we want to read IDListSize.
    if (fseek(fp, 48L, SEEK_CUR) != 0) {
    // The size of unnecessary value of HEADER is 48 bytes.
    // Which is sum of (8 + 8 + 8 + 4 + 4 + 4 + 2 + 2 + 4 + 4)bytes
    // (from "CreationTime" value to "Reserved3" value).
        printf("fail to move file pointer.\n");
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        fclose(fp);
        return 0;
    } else {
    // We modified file pointer's position successfully.
        fread(IDListSize, sizeof(WORD), 1, fp);
    // Read IDListSize value.
    }
    // We have to skip unnecessary value of LinkTargetIDList.
    // if we want to read some components of LinkInfo.
    if (fseek(fp, *IDListSize, SEEK_CUR) != 0) {
    // The size of unnecessary value of LinkTargetIDList is IDListSize.
        printf("fail to move file pointer.\n");
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        fclose(fp);
        return 0;
    }

    // Read LocalBasePathOffset which is size of difference
    // between the start position of LinkInfo and LocalBasePath.
    // Read CommonPathSuffixOffset which is size of difference
    // between the start position of LinkInfo and LocalBasePath.
    DWORD LocalBasePathO[1];
    // The array for saving LocalBasePathOffset value.
    DWORD CommonPathSuffixO[1];
    // The array for saving CommonPathSuffixOffset value.
    // We have to skip unnecessary value of LinkInfo.
    if (fseek(fp, 16L, SEEK_CUR) != 0) {
    // The size of unnecessary value is 16 bytes.
    // Which is sum of (4 + 4 + 4 + 4)bytes
    // (from "LinkInfoSize" value to "VolumeIDOffset" value).
        printf("fail to move file pointer.\n");
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        fclose(fp);
        return 0;
    } else {
    // We modified file pointer's position successfully.
        fread(LocalBasePathO, sizeof(DWORD), 1, fp);
    // Read LocalBasePathOffset value.
    }
    // We have to skip unnecessary value of LinkInfo.
    if (fseek(fp, 4L, SEEK_CUR) != 0) {
    // The size of unnecessary value is 4 bytes
    // which is size of "CommonNetworkRelativeLinkOffset" value.
        printf("fail to move file pointer.\n");
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        fclose(fp);
        return 0;
    } else {
    // We modified file pointer's position successfully.
        fread(CommonPathSuffixO, sizeof(DWORD), 1, fp);
    // Read CommonPathSuffixOffset value.
    }

    // Read LocalBasePath and CommonPathSuffix to know it's length.
    BYTE rdChar[1];          // The array to read binary of character as BYTE
    int sizeLBPBuffer = 0;   // The length of LocalBasePath(L.B.P.)
    int sizeCPSBuffer = 0;   // The length of CommonPathSuffix(C.P.S.)
    *pathName = (char)0;     // initialization of "pathName"
    // We have to go back to the start point of LinkInfo.
    if (fseek(fp, -28L, SEEK_CUR) != 0) {
        printf("fail to move file pointer.\n");
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        fclose(fp);
        return 0;
    }
    // Go to start point of LocalBasePath value.
    if (fseek(fp, *LocalBasePathO, SEEK_CUR) != 0) {
        printf("fail to move file pointer.\n");
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        fclose(fp);
        return 0;
    }

    while (1) {
    // Read binary of character until it is same as binary of "\0".
        fread(rdChar, sizeof(BYTE), 1, fp);
        if (!strncmp((char *)rdChar, "\0", 1)) {
            break;
        }
    // Count the length of LocalBasePath(exclude the length of "\0").
        sizeLBPBuffer++;
    }

    *rdChar = (char)0;   // initialization of "rdChar"

    // We have to go back to the start point of LinkInfo.
    if (fseek(fp, -(sizeLBPBuffer + 1), SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        printf("fail to move file pointer.\n");
        fclose(fp);
        return 0;
    }
    if (fseek(fp, -(*LocalBasePathO), SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        printf("fail to move file pointer.\n");
        fclose(fp);
        return 0;
    }
    // Go to start point of CommonPathSuffix value.
    if (fseek(fp, *CommonPathSuffixO, SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
        printf("fail to move file pointer.\n");
        fclose(fp);
        return 0;
    }

    while (1) {
    // Read binary of character until it is same as binary of "\0".
        fread(rdChar, sizeof(BYTE), 1, fp);
        if (!strncmp((char *)rdChar, "\0", 1)) {
            break;
        }
    // Count the length of CommonPathSuffix(exclude the length of "\0").
        sizeCPSBuffer++;
    }

    *rdChar = (char)0;   // initialization of "rdChar"

    int realSizePathName;
    // the integer variable of the sum of "sizeLBPBuffer" and "sizeCPSBuffer"
    realSizePathName = sizeLBPBuffer + sizeCPSBuffer;

    if (sizePathName < realSizePathName + 1) {
    // sizePathName(buffer) value is small than real size of pathname.
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, "Reading pathname from file fails due to ");
        fprintf(stderr, "small buffer size (%d).\n", sizePathName);
        fprintf(stderr, "[NOTE] ");
        fprintf(stderr, "We need buffer of size %d ", realSizePathName + 1);
        fprintf(stderr, "to hold target path name.");
        fclose(fp);
        return 0;
    } else {
    // sizePathName value is not small than real size of pathname.

    // We have to go back to the start point of LinkInfo.
        if (fseek(fp, -(sizeCPSBuffer + 1), SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
            printf("fail to move file pointer.\n");
            fclose(fp);
            return 0;
        }
        if (fseek(fp, -(*CommonPathSuffixO), SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
            printf("fail to move file pointer.\n");
            fclose(fp);
            return 0;
        }
    // Go to start point of LocalBasePath value.
        if (fseek(fp, *LocalBasePathO, SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
            printf("fail to move file pointer.\n");
            fclose(fp);
            return 0;
        }
    // Read binary of character for length of LocalBasePath.
        for (int i = 0; i < sizeLBPBuffer; i++) {
            fread(rdChar, sizeof(BYTE), 1, fp);
            strncat(pathName, (char *)rdChar, 1);
    // Save LocalBasePath to array which is pointed by pathName.
        }

    // We have to go back to the start point of LinkInfo.
        if (fseek(fp, -(sizeLBPBuffer), SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
            printf("fail to move file pointer.\n");
            fclose(fp);
            return 0;
        }
        if (fseek(fp, -(*LocalBasePathO), SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
            printf("fail to move file pointer.\n");
            fclose(fp);
            return 0;
        }
    // Go to start point of CommonPathSuffix value.
        if (fseek(fp, *CommonPathSuffixO, SEEK_CUR) != 0) {
    // We have to check file pointer because we modified it's position.
    // If it were not modified, print error message and return value "0".
            printf("fail to move file pointer.\n");
            fclose(fp);
            return 0;
        }
    // Read binary of character for length of CommonPathSuffix.
        for (int j = 0; j < sizeCPSBuffer; j++) {
            fread(rdChar, sizeof(BYTE), 1, fp);
            strncat(pathName, (char *)rdChar, 1);
    // Save CommonPathSuffix to array which is pointed by pathName.
        }
    }
    fclose(fp);
    return 1;
}
/*-------------------  shortcut.c -------------------------*/
