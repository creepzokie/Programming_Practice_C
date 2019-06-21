/**
 *    Copyright (c) 2016-2018 by Hyoung Bok Min, All rights reserved.
 *    For license information, please refer to
 *        http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *    Subject of LAB : Abstract Data Type SET by using Linked List.
 *
 *    This is the main program provided to students who registered
 *    in "EE Programming Lab" at School of Electrical Engineering,
 *    Sungkyunkwan University.
 *
 *    ================================================================
 *    You must use function "mymalloc()" for memory allocation, and
 *    you have to use function "myfree()" for disposing memory.
 *    DO NOT USE "malloc()/free()" FOR DYNAMIC MEMORY ALLOCATION.
 *    ================================================================
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#define  _MEMCHECK_H_SIZE_OVER 5000
#include "memcheck.h"

/**
 *    Constants for Large Set Union and Intersection
 */
#ifndef MAX_LARGE
#define MAX_LARGE    20000
#endif
#ifndef NUM_COMMONS
#define NUM_COMMONS  3
#endif

/**
 *    NODE data structure used in this program
 */
typedef struct list_node {
    struct list_node *link;   /* link */
    int data;
} NODE;


int isEmpty(NODE *set);
int isMember(NODE *set, int i_data);
int addElement(NODE **pset, int i_data);
int deleteElement(NODE **pset, int *pi_data);
NODE *setIntersection(NODE *set1, NODE *set2);
NODE *setUnion(NODE *set1, NODE *set2);
void deleteSet(NODE **pset);

// The following functions are given.
int printSet(NODE *set, int *expected, int size);
int sizeSet(NODE *set);
static void deleteArray(int *data_buf, int size, int i_data);
static int  useNewMemories(NODE *new_set, NODE *set1, NODE *set2);
static int  buildSets(NODE **set1, int *num1, NODE **set2, int *num2,
                      int num_large, int max_value,
                      int common_data[], int num_commons);
static int  valid_common_set(NODE *set, int common_data[], int num_commons);


int main(void) {
    NODE *set1 = 0;
    NODE *set2 = 0;
    NODE *union_set, *common_set, *item;
    int  *data_buf;    // temporary data area

    /* set 1 at phase 0 */
    int  data01[] = { 11, 18, 23, 43, 51, 65, 76, 89, 99 };
    /* set 2 at phase 0 */
    int  data02[] = { 17, 23, 43, 55, 65 };
    /* expected set of union at phase 0 */
    int  u_data0[] = { 11, 17, 18, 23, 43, 51, 55, 65, 76, 89, 99 };
    /* expected set of intersection at phase 0 */
    int  i_data0[] = { 23, 43, 65 };
    /* add sequence of set 1 at phase 0 */
    int  add_index0[] = { 7, 8, 2, 6, 0, 3, 5, 1, 4 };

    /* set 1 at phase 1 */
    int  data11[] = { 128, 256, 328, 415, 512 };
    /* set 2 at phase 1 */
    int  data12[] = { 202, 303, 404 };
    /* expected set of union at phase 1 */
    int  u_data1[] = { 128, 202, 256, 303, 328, 404, 415, 512 };
    /* expected set of intersection at phase 1 */
    int  *i_data1 = 0;
    /* add sequence of set 1 at phase 1 */
    int  add_index1[] = { 1, 3, 0, 2, 4 };

    int  *data_set1;
    int  *data_set2;
    int  *i_expected;
    int  *u_expected;
    int  *add_index;
    int  i, num1, num2, num_u, num_i, i_data, phase;
    int  retval = 0;
    clock_t start, stop;
    double  build_seconds, run_seconds;

    for (phase = 0 ; phase < 2 ; phase++) {
        /* Perform same operations twice for different data */
        if (phase == 0) {
            data_set1 = data01;
            num1 = sizeof(data01)/sizeof(int);
            data_set2 = data02;
            num2 = sizeof(data02)/sizeof(int);
            u_expected = u_data0;
            num_u = sizeof(u_data0)/sizeof(int);
            i_expected = i_data0;
            if (i_expected)
                num_i = sizeof(i_data0)/sizeof(int);
            else
                num_i = 0;
            add_index = add_index0;
            assert(sizeof(data01) == sizeof(add_index0));
        } else {
            data_set1 = data11;
            num1 = sizeof(data11)/sizeof(int);
            data_set2 = data12;
            num2 = sizeof(data12)/sizeof(int);
            u_expected = u_data1;
            num_u = sizeof(u_data1)/sizeof(int);
            i_expected = i_data1;
            if (i_expected)
                num_i = sizeof(i_data1)/sizeof(int);
            else
                num_i = 0;
            add_index = add_index1;
            assert(sizeof(data11) == sizeof(add_index1));
        }
        retval++;
        if (!isEmpty(set1) || !isEmpty(set2)) {
            fprintf(stderr, "ERROR: set 1 and set 2 should be empty.\n");
            return retval;
        }
        data_buf = (int *)mymalloc(num1*sizeof(int));
        assert(data_buf);

        /**
         *    [1] Create a set with addElement
         */
        printf("\nPerform addElement at phase %d\n", phase);
        retval++;
        for (i = 0 ; i < num1 ; i++) {
            data_buf[i] = data_set1[i];
            if (!addElement(&set1, data_set1[add_index[i]])) {
                fprintf(stderr, "ERROR: addElement failed (1).\n");
                return retval;
            }
        }
        retval++;
        if (addElement(&set1, data_set1[0])) {
            fprintf(stderr, "ERROR: addElement failed (2).\n");
            return retval;
        }
        printf("  addElement (Set 1) expected  :");
        for (i = 0 ; i < num1 ; i++)
            printf(" %d", data_set1[i]);
        printf("\n");
        printf("  addElement (Set 1) performed :");
        retval++;
        if (!printSet(set1, data_set1, num1)) {
            printf("\n");
            fprintf(stderr, "ERROR: addElement failed (3).\n");
            return retval;
        }
        printf("\n");

        /**
         *    [2] Delete an element by a given number
         */
        printf("\nPerform deleteElement at phase %d\n", phase);
        retval++;
        i_data = -1;
        if (deleteElement(&set1, &i_data)) {
            fprintf(stderr, "ERROR: deleteElement failed (1).\n");
            return retval;
        }
        retval++;
        for (i = 0 ; i < num1 ; i++) {
            i_data = data_set1[add_index[num1-1-i]];
            printf("  deleting an element '%d' :", i_data);
            if (!printSet(set1, data_buf, num1-i)) {
                printf("\n");
                fprintf(stderr, "ERROR: deleteElement failed (2).\n");
                return retval;
            }
            printf("\n");
            if (!deleteElement(&set1, &i_data)) {
                fprintf(stderr, "ERROR: deleteElement failed (3).\n");
                return retval+1;
            }
            deleteArray(data_buf, num1, i_data);
        }
        myfree(data_buf);
        retval += 2;
        if (deleteElement(&set1, (int *)0)) {
            fprintf(stderr, "ERROR: deleteElement failed (4).\n");
            return retval;
        }
        printf("  deleteElement performed okay.\n");

        /**
         *    [3] Create a set with addElement again
         */
        printf("\nPerform addElement again at phase %d\n", phase);
        retval++;
        for (i = 0 ; i < num1 ; i++) {
            if (!addElement(&set1, data_set1[add_index[i]])) {
                fprintf(stderr, "ERROR: addElement failed (6).\n");
                return retval;
            }
        }
        printf("  addElement expected  :");
        for (i = 0 ; i < num1 ; i++)
            printf(" %d", data_set1[i]);
        printf("\n");
        retval++;
        printf("  addElement performed :");
        if (!printSet(set1, data_set1, num1)) {
            printf("\n");
            fprintf(stderr, "ERROR: addElement failed (7).\n");
            return retval;
        }
        printf("\n");

        /**
         *    [4] Delete an element without specifying a number
         */
        printf("\nPerform deleteElement at phase %d\n", phase);
        retval++;
        for (i = 0 ; i < num1 ; i++) {
            printf("  deleting a smallest element :");
            if (!printSet(set1, &data_set1[i], num1-i)) {
                printf("\n");
                fprintf(stderr, "ERROR: deleteElement failed (6).\n");
                return retval;
            }
            printf("\n");
            if (!deleteElement(&set1, (int *)0)) {
                fprintf(stderr, "ERROR: deleteElement failed (7).\n");
                return retval+1;
            }
        }
        retval += 2;
        if (deleteElement(&set1, (int *)0)) {
            fprintf(stderr, "ERROR: deleteElement failed (8).\n");
            return retval;
        }
        printf("  deleteElement performed okay.\n");

        /**
         *    [5] Create two sets set1, set2 for union/intersection
         */
        printf("\nCreating Set 1 for set union/intersection\n");
        retval++;
        for (i = 0 ; i < num1 ; i++) {
            if (!addElement(&set1, data_set1[i])) {
                fprintf(stderr, "ERROR: addElement failed (11).\n");
                return retval;
            }
        }
        printf("  Set 1 expected  = {");
        for (i = 0 ; i < num1 ; i++)
            printf(" %d", data_set1[i]);
        printf(" }\n");
        retval++;
        printf("  Set 1 performed = {");
        if (!printSet(set1, data_set1, num1)) {
            printf(" }\n");
            fprintf(stderr, "ERROR: addElement failed (12).\n");
            return retval;
        }
        printf(" }\n");

        printf("\nCreating Set 2 for set union/intersection\n");
        retval++;
        for (i = 0 ; i < num2 ; i++) {
            if (!addElement(&set2, data_set2[i])) {
                fprintf(stderr, "ERROR: addElement failed (13).\n");
                return retval;
            }
        }
        printf("  Set 2 expected  = {");
        for (i = 0 ; i < num2 ; i++)
            printf(" %d", data_set2[i]);
        printf(" }\n");
        retval++;
        printf("  Set 2 performed = {");
        if (!printSet(set2, data_set2, num2)) {
            printf(" }\n");
            fprintf(stderr, "ERROR: addElement failed (14).\n");
            return retval;
        }
        printf(" }\n");

        /**
         *    [6] Set union
         */
        // union with empty set
        printf("\nUnion of Set1 and an Empty set at phase %d\n", phase);
        union_set = setUnion(set1, NULL);
        printf("  Set1 U Empty-set expected  = {");
        for (i = 0 ; i < num1 ; i++)
            printf(" %d", data_set1[i]);
        printf(" }\n");
        retval++;
        printf("  Set1 U Empty-set performed = {");
        if (!printSet(union_set, data_set1, num1)) {
            printf(" }\n");
            fprintf(stderr, "ERROR: union failed (1).\n");
            return retval;
        }
        printf(" }\n");
        deleteSet(&union_set);

        // union with empty set
        printf("\nUnion of an Empty set and Set2 at phase %d\n", phase);
        union_set = setUnion(NULL, set2);
        printf("  Empty-set U Set2 expected  = {");
        for (i = 0 ; i < num2 ; i++)
            printf(" %d", data_set2[i]);
        printf(" }\n");
        retval++;
        printf("  Empty-set U Set2 performed = {");
        if (!printSet(union_set, data_set2, num2)) {
            printf(" }\n");
            fprintf(stderr, "ERROR: union failed (2).\n");
            return retval;
        }
        printf(" }\n");
        deleteSet(&union_set);

        // union of non-empty sets
        printf("\nUnion of two sets, (Set1 and Set2) at phase %d\n", phase);
        union_set = setUnion(set1, set2);
        printf("  Set1 U Set2 expected  = {");
        for (i = 0 ; i < num_u ; i++) {
            printf(" %d", u_expected[i]);
        }
        printf(" }\n");
        retval++;
        printf("  Set1 U Set2 performed = {");
        if (!printSet(union_set, u_expected, num_u)) {
            fprintf(stderr, "ERROR: union failed (3).\n");
            return retval;
        }
        printf(" }\n");

        /* Check union */
        retval++;
        if (!useNewMemories(union_set, set1, set2)) {
            fprintf(stderr, "ERROR: Union of 2 sets uses memories ");
            fprintf(stderr, "in set1 and/or set2.\n");
            return retval;
        }
        retval++;
        item = set1;
        while (item) {
            if (!isMember(union_set, item->data)) {
                fprintf(stderr, "ERROR: %d in Set1 is", item->data);
                fprintf(stderr, " not a member of union\n");
                return retval;
            }
            item = item->link;
        }
        retval++;
        item = set2;
        while (item) {
            if (!isMember(union_set, item->data)) {
                fprintf(stderr, "ERROR: %d in Set2 is", item->data);
                fprintf(stderr, " not a member of union\n");
                return retval;
            }
            item = item->link;
        }

        /**
         *    [7] Set intersection
         */
        // intersection with empty set
        printf("\nIntersection of Set1 and an Empty set at phase %d\n", phase);
        common_set = setIntersection(set1, NULL);
        printf("  Set1 ^ Empty-set expected  = { }\n");
        printf("  Set1 ^ Empty-set performed = {");
        printSet(common_set, NULL, 0);
        printf(" }\n");
        retval++;
        if (!isEmpty(common_set)) {
            fprintf(stderr, "ERROR: intersection failed (1).\n");
            return retval;
        }
        deleteSet(&common_set);

        // intersection with empty set
        printf("\nIntersection of an Empty set and Set2 at phase %d\n", phase);
        common_set = setIntersection(NULL, set2);
        printf("  Empty-set ^ Set2 expected  = { }\n");
        printf("  Empty-set ^ Set2 performed = {");
        printSet(common_set, NULL, 0);
        printf(" }\n");
        retval++;
        if (!isEmpty(common_set)) {
            fprintf(stderr, "ERROR: intersection failed (2).\n");
            return retval;
        }
        deleteSet(&common_set);

        // intersection of non-empty sets
        printf("\nIntersection of two sets, (Set1 and Set2) at phase %d\n",
               phase);
        common_set = setIntersection(set1, set2);
        printf("  Set1 ^ Set2 expected  = {");
        for (i = 0 ; i < num_i ; i++) {
            printf(" %d", i_expected[i]);
        }
        printf(" }\n");
        retval++;
        printf("  Set1 ^ Set2 performed = {");
        if (!printSet(common_set, i_expected, num_i)) {
            fprintf(stderr, "ERROR: intersection failed (3).\n");
            return retval;
        }
        printf(" }\n");

        /* Check intersection */
        retval++;
        if (!useNewMemories(common_set, set1, set2)) {
            fprintf(stderr, "ERROR: Intersection of 2 sets uses memories ");
            fprintf(stderr, "in set1 and/or set2.\n");
            return retval;
        }

        retval++;
        item = common_set;
        while (item) {
            if (!isMember(set1, item->data)) {
                fprintf(stderr, "ERROR: %d in intersection is", item->data);
                fprintf(stderr, " not a member of Set1\n");
                return retval;
            }
            if (!isMember(set2, item->data)) {
                fprintf(stderr, "ERROR: %d in intersection is", item->data);
                fprintf(stderr, " not a member of Set2\n");
                return retval+1;
            }
            item = item->link;
        }

        /**
         *    [8] Delete the set, and perform the same operation once more.
         */
        printf("\nDeleting current sets at phase %d\n", phase);
        deleteSet(&set1);
        deleteSet(&set2);
        deleteSet(&union_set);
        deleteSet(&common_set);
        retval += 2;
        if (set1 || set2 || union_set || common_set) {
            fprintf(stderr, "ERROR: Sets are not deleted.\n");
            return retval;
        }
        printf("deleteSet performed okay\n");
    }

    /**
     *      Finally, set union / intersection for large sets
     */
    printf("\nLarge set intersection and union\n");
    printf("  Building Set1 and Set2 with random numbers.\n");
    printf("  It may take long time, please be patient ...\n");
    start = clock();   // measure time before generation
    assert(start >= 0);
    retval++;
    int common_data[NUM_COMMONS];
    if (!buildSets(&set1, &num1, &set2, &num2,
                   MAX_LARGE, 10*MAX_LARGE, common_data, NUM_COMMONS)) {
        return retval;
    }
    stop = clock();    // measure time after generation
    build_seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;  // CPU time
    printf("    CPU time: Build sets = %.2lf seconds\n", build_seconds);

    printf("  Intersecting the above sets ...\n");
    start = clock();   // measure time before intersection
    assert(start >= 0);

    common_set = setIntersection(set1, set2);

    stop = clock();    // measure time after intersection
    run_seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;  // CPU time
    printf("    CPU time: Set intersection = %.2lf seconds\n", run_seconds);

    printf("    Set1 ^ Set2 performed = {");
    printSet(common_set, NULL, 0);
    printf(" }\n");
    retval++;
    if (!valid_common_set(common_set, common_data, NUM_COMMONS)) {
        fprintf(stderr, "ERROR: Large set intersection failed.\n");
        return retval;
    }
    deleteSet(&common_set);

    printf("  Union of the above sets ...\n");
    start = clock();   // measure time before union
    assert(start >= 0);

    union_set = setUnion(set1, set2);

    stop = clock();    // measure time after union
    run_seconds = ((double)(stop - start)) / CLOCKS_PER_SEC;  // CPU time
    printf("    CPU time: Set Union = %.2lf seconds\n", run_seconds);

    printf("    Size of Set1 U Set2 = %d : ", sizeSet(union_set));
    printf("this number may vary depending on compilers\n");
    retval++;
    if (sizeSet(union_set) != (num1 + num2 - NUM_COMMONS)) {
        fprintf(stderr, "ERROR: Large set union failed.\n");
        return retval;
    }
    deleteSet(&union_set);

    printf("  Dispose memories ...\n");
    deleteSet(&set1);
    deleteSet(&set2);

    check_memory();
    printf("Program terminated okay (%d).\n", retval);
    return 0;
}


/**
 *    Print contents of a set.
 *    This function also compares the set with expected value array.
 *
 *    Returns 1 if set is the same as expected values array,
 *            0 otherwise.
 */
int printSet(NODE *set, int *expected, int size) {
    NODE *node = set;
    int  count = 0;
    int  okay = 1;

    while (node) {
        printf(" %d", node->data);
        if (expected) {
            if (count >= size)
                okay = 0;
            else if (node->data != expected[count])
                okay = 0;
        }
        count++;
        node = node->link;
    }
    if (expected && (count != size))
        okay = 0;
    return okay;
}

/**
 *    Given a set, returns size of the set.
 */
int sizeSet(NODE *set) {
    NODE *node = set;
    int  index = 0;

    while (node) {
        index++;
        node = node->link;
    }
    return index;
}

/**
 *    Given a set of integer array, delete the member from the set if exists.
 */
static void deleteArray(int *set, int size, int member) {
    int index, found = 0;

    for (index = 0 ; index < size ; index++) {
        if (found) {
            set[index] = set[index+1];
        } else if (set[index] == member) {
            found = 1;
            set[index] = set[index+1];
        }
    }
}

/**
 *    Given set1, set2, and a new_set, this function
 *    returns 0 if the new_set uses memories in set1 and/or set2,
 *            1 if the new_set uses completely new memories.
 */
static int useNewMemories(NODE *new_set, NODE *set1, NODE *set2) {
    NODE *new_node = new_set;

    while (new_node) {
        NODE *node = set1;
        while (node) {
            node = node->link;
            if (new_node == node)
                return 0;
        }
        node = set2;
        while (node) {
            node = node->link;
            if (new_node == node)
                return 0;
        }
        new_node = new_node->link;
    }
    return 1;
}

/**
 *    Generate 31 bit random number, 0 ~ 0x7fffffff.
 *    This is for Microsoft Windows where rand() generates 15 bit
 *    random numbers, which may be too small for this program.
 */
static int myrand(void) {
#ifdef _WIN32
    int num = (rand() & 0x7f) << 8;
    num = (num | (rand() & 0xff)) << 8;
    num = (num | (rand() & 0xff)) << 8;
    num = num | (rand() & 0xff);
    return num;
#else
    return rand();
#endif
}

/**
 *    Generate two large sets with a few common elements.
 *
 *    Inputs :
 *        int num_large  : Number of elements in the large sets.
 *        int max_value  : Elements of the large sets are integers between
 *                         0 and max_value-1.
 *        int num_commons: Number of common elements in the two sets.
 *
 *    Outputs :
 *        NODE **set1 : A large set generated by this function.
 *        int *num1   : Number of elements in set1.
 *        NODE **set2 : The other large set generated by this function.
 *        int *num2   : Number of elements in set2.
 *        int common_data[] : Array of common elements, which are in both set1
 *                            and set2. The intersection of the above 2 sets
 *                            should be the same as the elements in this array.
 */
static int buildSets(NODE **set1, int *num1, NODE **set2, int *num2,
                     int num_large, int max_value,
                     int common_data[], int num_commons) {
    // define common elements.
    int interval = max_value/(num_commons+1);
    assert(interval > 0);
    for (int i = 0 ; i < num_commons ; i++)
        common_data[i] = (i+1)*interval;

    // create 2 large disjoint sets.
    *num1 = *num2 = 0;
    for (int i = 0 ; i < num_large ; i++) {
        int i_data = myrand() % max_value;
        if (!(i_data%2))
            i_data++;   // make it even
        if (addElement(set1, i_data))
            (*num1)++;
        i_data = myrand() % max_value;
        if (i_data%2)
            i_data++;   // make it odd
        if (addElement(set2, i_data))
            (*num2)++;
    }
    // check size of the 2 sets.
    int size = sizeSet(*set1);
    if (size != (*num1)) {
        char *msg = "ERROR: (1) Mismatch of sizes of set 1, %d and %d. "
                    "Return value of function addElement() is invalid.\n";
        fprintf(stderr, msg, size, *num1);
        return 0;
    }
    size = sizeSet(*set2);
    if (size != (*num2)) {
        char *msg = "ERROR: (1) Mismatch of sizes of set 2, %d and %d. "
                    "Return value of function addElement() is invalid.\n";
        fprintf(stderr, msg, size, *num2);
        return 0;
    }

    // add common elements.
    for (int i = 0 ; i < num_commons ; i++) {
        if (addElement(set1, common_data[i])) {
            (*num1)++;
        }
        if (addElement(set2, common_data[i])) {
            (*num2)++;
        }
    }
    // check size of the 2 sets again.
    size = sizeSet(*set1);
    if (size != (*num1)) {
        char *msg = "ERROR: (2) Mismatch of sizes of set 1, %d and %d. "
                    "Return value of function addElement() is invalid.\n";
        fprintf(stderr, msg, size, *num1);
        return 0;
    }
    size = sizeSet(*set2);
    if (size != (*num2)) {
        char *msg = "ERROR: (2) Mismatch of sizes of set 2, %d and %d. "
                    "Return value of function addElement() is invalid.\n";
        fprintf(stderr, msg, size, *num2);
        return 0;
    }
    return 1;
}

/**
 *    Given a set,
 *    Returns 1 if the set matches precomputed common data,
 *            0 otherwise.
 */
static int valid_common_set(NODE *set, int common_data[], int num_commons) {
    if (sizeSet(set) != num_commons)
        return 0;
    NODE *item = set;
    for (int i = 0; i < num_commons ; i++) {
        if (item->data != common_data[i])
            return 0;
        item = item->link;
    }
    return 1;
}

/*--- set.c ---*/
