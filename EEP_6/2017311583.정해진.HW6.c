/**
 * Copyright (c) 2003-2018 by Hyoung Bok Min, All rights reserved.
 * Copyright (c) 2018 by Hae Jin Jung, All rights reserved.
 * For license information, please refer to
 *     http://class.icc.skku.ac.kr/~min/program/license.html
 *
 * Programming Lab.5 : Structures and Unions
 *
 * Summary of this lab :
 * (1) When the program starts, first ask for the name of the data file.
 * (2) If the user gives the file name, the circuit data contained in the file
 *     is read. If there is a problem reading the data file, report the problem
 *     and exit the program. If there is no problem, go to (3).
 * (3) The program asks the command to process. When the user inputs
 *     the command and presses the Enter key, the command is processed
 *     and goes to step (3) again.
 * (1) Read data from a binary data file and store it in the structure
 *     defined in "person.h"
 * (2) This file contains the personal information of several people
 *     who are named after their name, age, gender, gender, spouse's name,
 *     phone number, and the name of the city they live in.
 * (3) Write the read data to a text file
 * (4) Perform some calculations based on struct / union
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
 *    2. modified by Hae Jin Jung on May 14, 2018
 *       (a) Add codes for reading and writing text file (homeworks).
 *
 * This is the main program provided to students who registered
 * in "EE Programming Lab" at School of Electrical Engineering,
 * Sungkyunkwan University.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "person.h"
#define  _MEMCHECK_H_SIZE_OVER 100000
#include "memcheck.h"

int readDataFile(PERSON **, char *);
int writeTXT(PERSON *, int, char *);
int searchPersonINFO(PERSON *, int, char find[]);
void computeDATA(PERSON *, int);
void countSomethingCond1(PERSON *, int);
void countSomethingCond2(PERSON *, int);
void countSomethingCond3(PERSON *, int);
void disposeMemoryOfPersons(PERSON *, int);

/**
 *   Please note that main() is not a finished function.
 *   Although we said that you cannot modify codes at main() as a general rule,
 *   you may add your program codes at main() in this lab.
 */
int main(int argc, char **argv) {
    char *infile = "in.bin";    // default input binary data file
    char *outfile = "out.txt";  // default output text data file
    PERSON *persons;      // array of personal data
    int num_persons;      // number of persons in the data file
    char *find[] = {"Andy", "Laura", "Joshua"};  // names of persons to search

    /**
     *  Get input/output file names from command line if given.
     *  The 1st argument is used as input file, and the 2nd as output.
     */
    if (argc > 1) {
        infile = argv[1];
        if (argc > 2) {
            outfile = argv[2];
            if (argc > 3) {
                printf("[WARNING] The 3rd and later arguments are ignored.\n");
            }
        }
    }

    /**
     *    Read binary data file "in.bin",
     *    which contains personal data of many persons
     */
    printf("- Reading binary data file, \"%s\"\n", infile);
    num_persons = readDataFile(&persons, infile);
    if (num_persons <= 0) {
        // DO NOT REMOVE THE FOLLOWING LINE. IF YOU DO, YOU GET 'F'.
        check_memory();
        return 1;
    }

    /**
     *    Dump the above personal data into a text file
     */
    printf("- Writing text data file, \"%s\"\n", outfile);
    if (!writeTXT(persons, num_persons, outfile)) {
        disposeMemoryOfPersons(persons, num_persons);
        check_memory();
        return 1;
    }


    /**
     *    Search the 3 persons in "find[]" array,
     *    and print to the information to console.
     *    The print format is the same as a line in "out.txt"
     */
    for (int i = 0 ; i < 3 ; i++) {
        printf("- Searching \"%s\"\n", find[i]);
        if (!searchPersonINFO(persons, num_persons, find[i])) {
            disposeMemoryOfPersons(persons, num_persons);
            check_memory();
            return 1;
        }
    }

    /*
     *    Compute average and standard deviation of ages of all persons
     *    in the data file. Also, compute average heights of males and
     *    females, respectively.
     */
    printf("- Computing average and std. deviation of ages, and heights\n");
    computeDATA(persons, num_persons);

    /**
     *    How many ladies live in "NewYork" ?
     */
    printf("- How many ladies live in \"NewYork\" ?\n");
    countSomethingCond1(persons, num_persons);

    /**
     *    How many men have a phone number that begins with 777 ?
     */
    printf("- How many men have a phone that begins with 777 ?\n");
    countSomethingCond2(persons, num_persons);

    /**
     *    How many ladies have her names that begins with 'E'?
     *    What are the most frequent phone prefix(es) which are owned
     *    by husbands of the above ladies?
     */
    printf("- How many ladies have her name that begin with 'E'? ");
    printf("And, phone prefix?\n");
    countSomethingCond3(persons, num_persons);

    disposeMemoryOfPersons(persons, num_persons);

    /*
     *   DO NOT REMOVE THE FOLLOWING LINE.
     *   IF YOU DO, YOU GET 'F'.
     */
    check_memory();
    return 0;
}

int readDataFile(PERSON **p_persons, char *infile) {
    // open input file by binary reading mode
    FILE *fpb;
    fpb = myfopen(infile, "rb");
    if (!fpb) {
        myfclose(fpb);
        fprintf(stderr, "[ERROR] File '%s' not found.\n", infile);
        return 0;
    }
    // read a number of components
    int read_num_person;
    if (fread(&read_num_person, sizeof(int), 1, fpb) == 0) {
    // if reading fails, output the error message.
        myfclose(fpb);
        fprintf(stderr, "[ERROR] ");
        fprintf(stderr, "'%s' doesn't have a number of people.\n", infile);
        return 0;
    }
    // setting variables of saving data of binary file.
    short nameLength;
    char *save_name;
    short Age;
    double height;
    GENDER gender;
    short *STHLength;
    char **save_sth;
    unsigned phoneNum;

    int readTYPE;
    int errorTYPE = 0;
    int index = 0;

    STHLength = (short *)mymalloc(read_num_person * sizeof(short));
    save_sth = (char **)mymalloc(read_num_person * sizeof(char *));
    assert(save_sth);
    for (int index = 0; index < read_num_person; index++) {
        save_sth[index] = NULL;
    }
    PERSON *persons = (PERSON *)mymalloc(read_num_person *
                                         sizeof(PERSON));
    // start reading data chunks of each person
    while (index < read_num_person) {
    // read a length of Nth component's name
        nameLength = 0;
        if (fread(&nameLength, sizeof(short), 1, fpb) == 0) {
    // if reading fails, output the error message.
            myfclose(fpb);
            fprintf(stderr, "[ERROR] ");
            fprintf(stderr, "'%s' may not have a length of the ", infile);
            fprintf(stderr, "%dth person's name.\n", index + 1);
            if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another data chunk is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        }
    // read Nth person's name
    // read one letter first because of determining error or not
        save_name = (char *)mymalloc((nameLength + 1) * sizeof(char));
        assert(save_name);
        if (!(fread(save_name, sizeof(char), nameLength, fpb) == nameLength)) {
    // if reading fails, output the error message.
            myfclose(fpb);
            fprintf(stderr, "[ERROR] '%s' doesn't have the ", infile);
            fprintf(stderr, "%dth person's name or ", index + 1);
            fprintf(stderr, "has the invalid ");
            fprintf(stderr, "%dth person's name.\n", index + 1);
            myfree(save_name);
            if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another data chunk is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        } else {
            save_name[nameLength] = '\0';
            int checkAlpha = 0;
            for (int index = 0; index < nameLength; index++) {
                if (!(isalpha(save_name[index]))) {
                    checkAlpha++;
                }
            }
            if (!(checkAlpha == 0)) {
    // if reading fails, output the error message.
                myfclose(fpb);
                fprintf(stderr, "[ERROR] '%s' doesn't have the ", infile);
                fprintf(stderr, "%dth person's name or ", index + 1);
                fprintf(stderr, "has the invalid ");
                fprintf(stderr, "%dth person's name.\n", index + 1);
                myfree(save_name);
                if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another data chunk is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            }
            persons[index].name = (char *)mymalloc((strlen(save_name) + 1) *
                                                   sizeof(char));
            assert(persons[index].name);
            strncpy(persons[index].name, save_name, (strlen(save_name) + 1));
            myfree(save_name);
        }
    // read Nth person's age
        Age = 0;
        if (fread(&Age, sizeof(short), 1, fpb) == 0) {
    // if reading fails, output the error message.
            myfclose(fpb);
            fprintf(stderr, "[ERROR] '%s' doesn't have the ", infile);
            fprintf(stderr, "%dth person's age.\n", index + 1);
            myfree(persons[index].name);
            if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another data chunk is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        } else {
            if ((Age < 0) || (Age > 150)) {
    // if invalid age, output the error message.
                myfclose(fpb);
                fprintf(stderr, "[ERROR] '%s' has the invalid ", infile);
                fprintf(stderr, "%dth person's age.\n", index + 1);
                myfree(persons[index].name);
                if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another data chunk is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            } else {
                persons[index].age = Age;
            }
        }
    // read Nth person's height
        height = 0.0;
        if (fread(&height, sizeof(double), 1, fpb) == 0) {
    // if reading fails, output the error message.
            myfclose(fpb);
            fprintf(stderr, "[ERROR] '%s' doesn't have the ", infile);
            fprintf(stderr, "%dth person's height.\n", index + 1);
            myfree(persons[index].name);
            if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another data chunk is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        } else {
            if (fpclassify(height) < 1024) {
    // if reading fails, output the error message.
                myfclose(fpb);
                fprintf(stderr, "[ERROR] '%s' has the invalid ", infile);
                fprintf(stderr, "%dth person's height.\n", index + 1);
                myfree(persons[index].name);
                if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another data chunk is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            } else {
                persons[index].height = height;
            }
        }
    // read Nth person's gender
        if (fread(&gender, sizeof(GENDER), 1, fpb) == 0) {
    // if reading fails, output the error message.
            myfclose(fpb);
            fprintf(stderr, "[ERROR] '%s' doesn't have the ", infile);
            fprintf(stderr, "%dth person's gender.\n", index + 1);
            myfree(persons[index].name);
            if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another data chunk is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        } else {
            persons[index].gender = gender;
            if (gender == MALE) {
                readTYPE = 1;
            } else if (gender == FEMALE) {
                readTYPE = 2;
            } else {
                readTYPE = 0;
            }
        }
    // start reading data
    // which are different as the Nth person's gender
        if (readTYPE == 0) {
            myfclose(fpb);
            fprintf(stderr, "[ERROR] '%s' has invalid gender of ", infile);
            fprintf(stderr, "the %dth person.\n", index + 1);
            myfree(persons[index].name);
            if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                errorTYPE = 1;
                break;
            } else {
    // if another data chunk is invalid, error type is 2.
                errorTYPE = 2;
                break;
            }
        } else if (readTYPE == 1) {
    // read a length of name of the Nth person's spouse
            if (fread(&STHLength[index], sizeof(short), 1, fpb) == 0) {
    // if reading fails, output the error message.
                myfclose(fpb);
                fprintf(stderr, "[ERROR] '%s' doesn't have ", infile);
                fprintf(stderr, "a length of name ");
                fprintf(stderr, "of the %dth person's spouse.\n", index + 1);
                myfree(persons[index].name);
                if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another data chunk is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            }
            short sthLength = 0;
            sthLength = STHLength[index];
            if (STHLength[index] != 0) {
    // read a name of the Nth person's spouse
                save_sth[index] = (char *)mymalloc((sthLength + 1) *
                                                   sizeof(char));
                if (fread(save_sth[index],
                          sizeof(char),
                          STHLength[index], fpb) == 0) {
    // if reading fails, output the error message.
                    myfclose(fpb);
                    fprintf(stderr, "[ERROR] '%s' doesn't have ", infile);
                    fprintf(stderr, "a name of ");
                    fprintf(stderr, "the %dth person's spouse.\n", index + 1);
                    myfree(persons[index].name);
                    myfree(save_sth[index]);
                    if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                        errorTYPE = 1;
                        break;
                    } else {
    // if another data chunk is invalid, error type is 2.
                        errorTYPE = 2;
                        break;
                    }
                } else {
                    save_sth[index][sthLength] = '\0';
                    int checkAlpha = 0;
                    for (int index2 = 0; index2 < sthLength; index2++) {
                        if (!(isalpha(save_sth[index][index2]))) {
                            checkAlpha++;
                        }
                    }
                    if (!(checkAlpha == 0)) {
    // if reading fails, output the error message.
                        myfclose(fpb);
                        fprintf(stderr, "[ERROR] '%s' doesn't have ", infile);
                        fprintf(stderr, "a name of ");
                        fprintf(stderr, "the %dth person's spouse", index + 1);
                        fprintf(stderr, " or has the invalid name of the ");
                        fprintf(stderr, "%dth person's spouse.\n", index + 1);
                        myfree(persons[index].name);
                        myfree(save_sth[index]);
                        if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                            errorTYPE = 1;
                            break;
                        } else {
    // if another data chunk is invalid, error type is 2.
                            errorTYPE = 2;
                            break;
                        }
                    }
                }
            }
    // read the Nth person's phone number
            phoneNum = 0;
            if (fread(&phoneNum, sizeof(unsigned), 1, fpb) == 0) {
    // if reading fails, output the error message.
                myfclose(fpb);
                fprintf(stderr, "[ERROR] '%s' doesn't have ", infile);
                fprintf(stderr, "the phone number ");
                fprintf(stderr, "of the %dth person.\n", index + 1);
                myfree(persons[index].name);
                myfree(save_sth[index]);
                if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another data chunk is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            } else {
                if ((phoneNum < 11111111) || (phoneNum > 99999999)) {
                    myfclose(fpb);
                    fprintf(stderr, "[ERROR] '%s' has a ", infile);
                    fprintf(stderr, "invalid phone number ");
                    fprintf(stderr, "of the %dth person.\n", index + 1);
                    myfree(persons[index].name);
                    myfree(save_sth[index]);
                    if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                        errorTYPE = 1;
                        break;
                    } else {
    // if another data chunk is invalid, error type is 2.
                        errorTYPE = 2;
                        break;
                    }
                } else {
                    persons[index].info.phone_no = phoneNum;
                }
            }
        } else {
    // read a length of name of city the Nth person lives
            if (fread(&STHLength[index], sizeof(short), 1, fpb) == 0) {
    // if reading fails, output the error message.
                myfclose(fpb);
                fprintf(stderr, "[ERROR] '%s' doesn't have ", infile);
                fprintf(stderr, "a length of name of city ");
                fprintf(stderr, "the %dth person lives.\n", index + 1);
                myfree(persons[index].name);
                if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another data chunk is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            }
            short sthLength = 0;
            sthLength = STHLength[index];
    // read a name of city the Nth person lives
            save_sth[index] = (char *)mymalloc((STHLength[index] + 1) *
                                               sizeof(char));
            if (fread(save_sth[index],
                      sizeof(char),
                      STHLength[index], fpb) == 0) {
    // if reading fails, output the error message.
                myfclose(fpb);
                fprintf(stderr, "[ERROR] '%s' doesn't have name of ", infile);
                fprintf(stderr, "city the %dth person lives.\n", index + 1);
                myfree(persons[index].name);
                myfree(save_sth[index]);
                if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                    errorTYPE = 1;
                    break;
                } else {
    // if another data chunk is invalid, error type is 2.
                    errorTYPE = 2;
                    break;
                }
            } else {
                save_sth[index][sthLength] = '\0';
                int checkAlpha = 0;
                for (int index2 = 0; index2 < sthLength; index2++) {
                    if (!(isalpha(save_sth[index][index2]))) {
                        checkAlpha++;
                    }
                }
                if (!(checkAlpha == 0)) {
    // if reading fails, output the error message.
                    myfclose(fpb);
                    fprintf(stderr, "[ERROR] '%s' doesn't have ", infile);
                    fprintf(stderr, "a name of city ");
                    fprintf(stderr, "the %dth person lives ", index + 1);
                    fprintf(stderr, "or has the invalid name of the ");
                    fprintf(stderr, "%dth person lives.\n", index + 1);
                    myfree(persons[index].name);
                    myfree(save_sth[index]);
                    if (index == 0) {
    // if first data chunk is invalid, error type is 1.
                        errorTYPE = 1;
                        break;
                    } else {
    // if another data chunk is invalid, error type is 2.
                        errorTYPE = 2;
                        break;
                    }
                }
                persons[index].info.city = (char *)mymalloc((sthLength + 1) *
                                                            sizeof(char));
                assert(persons[index].info.city);
                strcpy(persons[index].info.city, save_sth[index]);
            }
        }
        index++;
    }
    if (errorTYPE == 1) {
    // if error type is 1, nothing to free memory of data read successfully
        myfree(persons);
        myfree(save_sth);
        myfree(STHLength);
        return 0;
    } else if (errorTYPE == 2) {
    // if error type is 2, free memory of data read successfully
        for (int i = 0; i < index; i++) {
            myfree(persons[i].name);
            if (STHLength[i] != 0) {
                myfree(save_sth[i]);
            }
            if (persons[i].gender == FEMALE) {
                myfree(persons[i].info.city);
            }
        }
        myfree(persons);
        myfree(save_sth);
        myfree(STHLength);
        return 0;
    } else {
        for (int i = 0; i < read_num_person; i++) {
            if (STHLength[i] == 0) {
                persons[i].spouse = NULL;
            }
        }
        for (int i = 0; i < read_num_person; i++) {
            for (int j = 1; j < read_num_person; j++) {
                if (!strcmp(save_sth[i], persons[j].name)) {
                    persons[i].spouse = &persons[j];
                    persons[j].spouse = &persons[i];
                }
            }
        }
        for (int i = 0; i < index; i++) {
            myfree(save_sth[i]);
        }
        myfree(save_sth);
        myfree(STHLength);
    // close input and output files
        myfclose(fpb);
    // points "p_persons" to "persons" for saving value globally
        *p_persons = persons;
    // return "name_array" array
        return read_num_person;
    }
}

int writeTXT(PERSON *persons, int num_persons, char *outfile) {
    // open output file by text writing mode
    FILE *fpo;
    fpo = myfopen(outfile, "w");
    if (!fpo) {
        myfclose(fpo);
        fprintf(stderr, "[ERROR] File '%s' cannot be created.\n", outfile);
        return 0;
    }
    // write a number of components
    fprintf(fpo, "%d\n", num_persons);
    // start writing data of each components
    for (int index = 0; index < num_persons; index++) {
    // write Nth person's name
        fprintf(fpo, "%s ", persons[index].name);
    // write Nth person's age
        fprintf(fpo, "%d ", persons[index].age);
    // write Nth person's height
        fprintf(fpo, "%.2f ", persons[index].height);
    // write Nth person's gender
        if (persons[index].gender == MALE) {
            fprintf(fpo, "M ");
        } else if (persons[index].gender == FEMALE) {
            fprintf(fpo, "F ");
        }
    // start writing data
    // which are different as the Nth person's gender
        if (persons[index].gender == MALE) {
            if (!persons[index].spouse) {
                fprintf(fpo, "%u", persons[index].info.phone_no);
            } else {
                fprintf(fpo, "%s ", persons[index].spouse->name);
                fprintf(fpo, "%u", persons[index].info.phone_no);
            }
        } else if (persons[index].gender == FEMALE) {
            fprintf(fpo, "%s", persons[index].info.city);
        }
        fprintf(fpo, "\n");
    }
    myfclose(fpo);
    return 1;
}

int searchPersonINFO(PERSON *persons, int num_persons, char *find) {
    int countSuccess = 0;
    for (int index = 0; index < num_persons; index++) {
        if (!(strncmp(persons[index].name, find, strlen(find)))) {
    // start writing data of specific person
    // write the person's name
            fprintf(stdout, "%s ", persons[index].name);
    // write the person's age
            fprintf(stdout, "%d ", persons[index].age);
    // write the person's height
            fprintf(stdout, "%.2f ", persons[index].height);
    // write the person's gender
    // and writing data which are different as the person's gender
            if (persons[index].gender == MALE) {
                fprintf(stdout, "M ");
                if (!persons[index].spouse) {
                    fprintf(stdout, "%u", persons[index].info.phone_no);
                } else {
                    fprintf(stdout, "%s ", persons[index].spouse->name);
                    fprintf(stdout, "%u", persons[index].info.phone_no);
                }
            } else if (persons[index].gender == FEMALE) {
                fprintf(stdout, "F ");
                fprintf(stdout, "%s", persons[index].info.city);
            }
            countSuccess++;
        }
    }
    if (countSuccess == 0) {
        fprintf(stderr, "[ERROR] File doesn't have data of %s.\n", find);
        return 0;
    }
    fprintf(stdout, "\n");
    return 1;
}

void computeDATA(PERSON *persons, int num_persons) {
    int sumAge = 0;
    double avgAge = 0.0;
    double sumSubtract = 0.0;
    double stdDev = 0.0;
    for (int index = 0; index < num_persons; index++) {
        sumAge += (int)persons[index].age;
    }
    avgAge = (double)sumAge / (double)num_persons;
    for (int index = 0; index < num_persons; index++) {
        sumSubtract += pow(((double)persons[index].age - avgAge), 2);
    }
    stdDev = sqrt((sumSubtract / (double)num_persons));
    fprintf(stdout, "average of age = %.2f\n", avgAge);
    fprintf(stdout, "std_dev of age = %.2f\n", stdDev);
    double sumHeightM = 0.0;
    double sumHeightF = 0.0;
    int countM = 0;
    int countF = 0;
    for (int index = 0; index < num_persons; index++) {
        if (persons[index].gender == MALE) {
            sumHeightM += persons[index].height;
            countM++;
        }
        if (persons[index].gender == FEMALE) {
            sumHeightF += persons[index].height;
            countF++;
        }
    }
    fprintf(stdout, "average height of males = ");
    fprintf(stdout, "%.2f\n", sumHeightM / (double)countM);
    fprintf(stdout, "average height of females = ");
    fprintf(stdout, "%.2f\n", sumHeightF / (double)countF);
}

void countSomethingCond1(PERSON *persons, int num_persons) {
    int countNewYorkFemale = 0;
    for (int index = 0; index < num_persons; index++) {
        if (persons[index].gender == FEMALE) {
            if (!(strncmp(persons[index].info.city, "NewYork", 7))) {
                countNewYorkFemale++;
            }
        }
    }
    fprintf(stdout, "%d ladies live in NewYork.\n", countNewYorkFemale);
}

void countSomethingCond2(PERSON *persons, int num_persons) {
    int countPhoneNumStart777 = 0;
    for (int index = 0; index < num_persons; index++) {
        if (persons[index].gender == MALE) {
            if (((persons[index].info.phone_no -
                  (unsigned)77700000) < 100000)
                && ((persons[index].info.phone_no -
                     (unsigned)77700000) >= 0)) {
                countPhoneNumStart777++;
            }
        }
    }
    fprintf(stdout, "%d men have a phone ", countPhoneNumStart777);
    fprintf(stdout, "that begins with 777.\n");
}

void countSomethingCond3(PERSON *persons, int num_persons) {
    int countFNameStartE = 0;
    for (int index = 0; index < num_persons; index++) {
        if (persons[index].gender == FEMALE) {
            if (!strncmp(persons[index].name, "E", 1)) {
                countFNameStartE++;
            }
        }
    }
    fprintf(stdout, "%d ladies have her names ", countFNameStartE);
    fprintf(stdout, "that begin with 'E'.\n");
    unsigned *savePNum = (unsigned *)mymalloc(countFNameStartE *
                                              sizeof(unsigned));
    int pNumIndex = 0;
    for (int index = 0; index < num_persons; index++) {
        if (persons[index].gender == FEMALE) {
            if (!strncmp(persons[index].name, "E", 1)) {
                savePNum[pNumIndex] = persons[index].spouse->info.phone_no;
                pNumIndex++;
            }
        }
    }
    int *savePNum3 = (int *)mymalloc(countFNameStartE *
                                     sizeof(unsigned));
    for (int index = 0; index < countFNameStartE; index++) {
        savePNum3[index] = (int)savePNum[index] / 100000;
    }
    int countUniqueNum;
    countUniqueNum = countFNameStartE;
    for (int index = 0; index < countFNameStartE; index++) {
        for (int index2 = index + 1; index2 < countFNameStartE; index2++) {
            if (savePNum3[index] == savePNum3[index2]) {
                countUniqueNum--;
            }
        }
    }
    int *uniqueArray = (int *)mymalloc(countUniqueNum * sizeof(int));
    for (int index = 0; index < countUniqueNum; index++) {
        uniqueArray[index] = 0;
    }
    int countSuccess = 0;
    int countOverlap = 0;
    for (int index = 0; index < countFNameStartE; index++) {
        if (countSuccess == countUniqueNum) {
            break;
        } else {
            for (int index2 = 0; index2 < countUniqueNum; index2++) {
                if (uniqueArray[index2] == savePNum3[index]) {
                    countOverlap++;
                }
            }
            if (countOverlap == 0) {
                uniqueArray[countSuccess] = savePNum3[index];
                countSuccess++;
            }
            countOverlap = 0;
        }
    }
    int *countUniq = (int *)mymalloc(countUniqueNum * sizeof(int));
    for (int index = 0; index < countUniqueNum; index++) {
        countUniq[index] = 0;
    }
    int saveCountOverlap = 0;
    for (int index = 0; index < countUniqueNum; index++) {
        for (int index2 = 0; index2 < countFNameStartE; index2++) {
            if (savePNum3[index2] == uniqueArray[index]) {
                saveCountOverlap++;
            }
        }
        countUniq[index] = saveCountOverlap;
        saveCountOverlap = 0;
    }
    int check1st = 0, check2nd = 0;
    for (int index = 0; index < countUniqueNum; index++) {
        if (check1st == 0) {
            if (countUniq[index] > 1) {
                check1st = uniqueArray[index];
            }
        } else if (check2nd == 0) {
            if (countUniq[index] > 1) {
                check2nd = uniqueArray[index];
                break;
            }
        }
    }
    fprintf(stdout, "%d, %d are the most ", check1st, check2nd);
    fprintf(stdout, "frequent phone prefix hold by their husbands.\n");
    myfree(savePNum);
    myfree(savePNum3);
    myfree(uniqueArray);
    myfree(countUniq);
}

void disposeMemoryOfPersons(PERSON *persons, int num_persons) {
    for (int i = 0; i < num_persons; i++) {
        myfree(persons[i].name);
        if (persons[i].gender == FEMALE) {
            myfree(persons[i].info.city);
        }
    }
    myfree(persons);
}

/*----- person.c -----*/
