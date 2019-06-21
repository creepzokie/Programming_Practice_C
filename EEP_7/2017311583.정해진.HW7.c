/**
 *  Copyright (c) 2003-2017 by Hyoung Bok Min, All rights reserved.
 *  Copyright (C) 2018 by Hae Jin Jung, All rights reserved.
 *  For license information, please refer to
 *      http://class.icc.skku.ac.kr/~min/program/license.html
 *
 *  complier used : GNU GCC Compiler
 *
 *  Lab 7 of C Programming : Linked List
 *
 *  Summary of this lab :
 *  1. linked_list.c is given
 *  2. void printList(NODE *) is given
 *  3. Write the other 5 functions at lab hours.
 *  4. Read the program codes as a pre-lab work.
 *  [function operation]
 *  - Insert an item to head of a list
 *  - Delete an item from head of a list
 *  - Insert an item to tail of a list
 *  - Delete an item next to head item
 *  - Delete all the items in a list
 *  - Print data of a list
 *
 *  Written by Jung, Hae Jin,
 *             School of Electrical and Electronics Engineering
 *             Sungkyunkwan University.
 *
 *  File name : 2017311583.JungHaeJin.HW7.c
 *
 *  Written on September 01, 2011
 *  Modification History :
 *     1. Wrtten by Hyoung Bok Min on August 19, 2012 (given as a template)
 *     2. modified by Hae Jin Jung on May 28, 2018
 *        (a) Add codes for reading and writing text file (homeworks).
 *
 *  This is the main program provided to students who registered
 *  in "Programming Lab" at School of Information and Communication,
 *  Sungkyunkwan University.
 *
 * ================================================================
 * You must use function "mymalloc()" for memory allocation, and
 * you have to use function "myfree()" for disposing memory.
 * DO NOT USE "malloc()/free()" FOR DYNAMIC MEMORY ALLOCATION.
 *
 *    void *mymalloc(unsigned int byte_count);
 *    void myfree(void *ptr);
 * ================================================================
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define  _MEMCHECK_H_SIZE_OVER 5000
#include "memcheck.h"

/**
 *    NODE data structure used in this program
 */
typedef struct list_node {
    struct list_node *link;
    int data;
} NODE;


int headInsert(NODE **head, NODE **tail, int data);
int headDelete(NODE **head, NODE **tail, int *data);
int tailInsert(NODE **head, NODE **tail, int data);
int secondDelete(NODE **head, NODE **tail, int *data);
void deleteList(NODE **head, NODE **tail);
static void printList(NODE *head);


int main(void) {
    NODE *head = 0;    // head pointer of linked list
    NODE *tail = 0;    // tail pointer of linked list
    int  data0[] = { 99, 89, 76, 65, 51, 43, 23, 18, 9 };
    int  data1[] = { 512, 415, 328, 256, 128 };
    int  *data_used;   // either data0 or data1
    int  num;          // number of data in data_used
    int  loop;         // loop variable
    int  i_data;       // integer data of linked list
    int  phase;        // we will perform same job twice in phase 0 and 1

    for (phase = 0 ; phase < 2 ; phase++) {
        if (phase) {
            data_used = data1;      // This data set is used in phase 1
            num = sizeof(data1)/sizeof(int);
        } else {
            data_used = data0;      // This data set is used in phase 0
            num = sizeof(data0)/sizeof(int);
        }

        /**
         *    [1] Create a list with headInsert
         */
        printf("\nPerform headInsert at phase %d\n", phase);
        for (loop = 0 ; loop < num ; loop++) {
            if (!headInsert(&head, &tail, data_used[loop])) {
                fprintf(stderr, "[E1] Head insertion failed(1).\n");
                return 1;
            }
            printf("  a node added to head :");
            printList(head);
            printf("\n");
        }
        if (tail->link) {
            fprintf(stderr, "[E2] Head insertion failed(2).\n");
            return 2;
        }
        printf("  headInsert expected  :");
        for (loop = num-1 ; loop >= 0 ; loop--)
            printf(" %d", data_used[loop]);
        printf("\n");
#ifdef PRINT_RESULT
        /*
         *    This block is for grading purpose only. Leave this block as was.
         */
        printf("  headInsert performed :");
        printList(head);
        printf("\n");
#endif

        /**
         *    [2] Delete head nodes
         */
        printf("\nPerform headDelete at phase %d\n", phase);
        for (loop = num-1 ; loop >= 0 ; loop--) {
            printf("  deleting a head node :");
            printList(head);
            printf("\n");
            if (!headDelete(&head, &tail, &i_data)) {
                fprintf(stderr, "[E3] Head deletion failed (1).\n");
                return 3;
            }
            if (i_data != data_used[loop]) {
                fprintf(stderr, "[E4] Head deletion failed (2).\n");
                return 4;
            }
        }
        if (headDelete(&head, &tail, &i_data)) {
            fprintf(stderr, "[E5] Head deletion failed (3).\n");
            return 5;
        }
        printf("  headDelete performed okay.\n");

        /**
         *    [3] Create a list with tailInsert
         */
        printf("\nPerform tailInsert at phase %d\n", phase);
        for (loop = 0 ; loop < num ; loop++) {
            if (!tailInsert(&head, &tail, data_used[loop])) {
                fprintf(stderr, "[E6] Tail insertion failed(1).\n");
                return 6;
            }
            printf("  a node added to tail :");
            printList(head);
            printf("\n");
        }
        if (tail->link) {
            fprintf(stderr, "[E7] Tail insertion failed(2).\n");
            return 7;
        }
        printf("  tailInsert expected  :");
        for (loop = 0 ; loop < num ; loop++)
            printf(" %d", data_used[loop]);
        printf("\n");

#ifdef PRINT_RESULT
        /*
         *    This block is for grading purpose only. Leave this block as was.
         */
        printf("  tailInsert performed :");
        printList(head);
        printf("\n");
#endif

        /**
         *    [4] Delete next to head nodes by using secondDelete()
         */
        printf("\nPerform secondDelete at phase %d\n", phase);
        for (loop = 0 ; loop < num ; loop++) {
            int expected_index = loop+1;
            if (loop == (num-1))
                expected_index = 0;
            printf("  deleting the next-to-head node :");
            printList(head);
            printf("\n");
            if (!secondDelete(&head, &tail, &i_data)) {
                fprintf(stderr, "[E8] Second deletion failed (1).\n");
                return 8;
            }
            if (i_data != data_used[expected_index]) {
                fprintf(stderr, "[E9] Second deletion failed (2).\n");
                return 9;
            }
        }
        if (secondDelete(&head, &tail, &i_data)) {
            fprintf(stderr, "[E10] Second deletion failed (3).\n");
            return 10;
        }
        printf("  secondDelete performed okay.\n");

        /**
         *    [5] Test deleteList()
         */
        // Build a linked list for deletion
        for (loop = 0 ; loop < num ; loop++) {
            if (!headInsert(&head, &tail, data_used[loop])) {
                fprintf(stderr, "[E11] Head insertion failed(3).\n");
                return 11;
            }
        }
        if (tail->link) {
            fprintf(stderr, "[E12] Head insertion failed(4).\n");
            return 12;
        }
        // Delete the list
        deleteList(&head, &tail);

        /**
         *    [6] Check the list, and perform the same operation once more.
         */
        if (head || tail) {
            fprintf(stderr, "[E13] List is not properly deleted.\n");
            return 13;
        }
        printf("End of this phase.\n");
    }

    /**
     *    [7] Epilog : check memory used by this program.
     *        WARNING: DO NOT REMOVE THE FOLLOWING FUNCTION CALL.
     */
    check_memory();
    return 0;
}

/**
 *   Given head and tail of the linked list and data to put in the list,
 *   insert data into the linked list.
 *   Insert data into the head of list first.
 */

int headInsert(NODE **head_pointer, NODE **tail_pointer, int data) {
    // memory allocation of saving input data which is composed of an integer
    // and address of the next data chuck.
    NODE *itemHEAD = (NODE *)mymalloc(sizeof(NODE));
    // alert error if memory allocation fails.
    if (!itemHEAD) {
        fprintf(stderr, "[ERROR] Out of memory.\n");
        return 0;
    }
    // input the integer
    itemHEAD->data = data;
    // input the address of pointer
    itemHEAD->link = *head_pointer;
    // if data chunk is one, tail pointer points to first data chuck
    if (!(*head_pointer)) {
        *tail_pointer = itemHEAD;
    }
    // link data chunks
    *head_pointer = itemHEAD;
    return 1;
}

/**
 *   Given head and tail of the linked list and data to delete,
 *   delete data in the linked list.
 *   Delete data in the head of list first.
 */

int headDelete(NODE **head_pointer, NODE **tail_pointer, int *data) {
    // set head data to ready to delete data
    NODE *itemHEAD = *head_pointer;
    if (!itemHEAD) {
        return 0;
    }
    // cut a link of two adjacent data chunks
    *data = itemHEAD->data;
    // link a data chunk and subsequent data chunk
    *head_pointer = itemHEAD->link;
    // if data chunk is one, set tail pointer also 0
    if (!(*head_pointer)) {
        *tail_pointer = 0;
    }
    // free the memory chunk
    myfree(itemHEAD);
    return 1;
}

/**
 *   Given head and tail of the linked list and data to put in the list,
 *   insert data into the linked list.
 *   Insert data into the tail of list first.
 */

int tailInsert(NODE **head_pointer, NODE **tail_pointer, int data) {
    // memory allocation of saving input data which is composed of an integer
    // and address of the next data chuck.
    NODE *itemTAIL = (NODE *)mymalloc(sizeof(NODE));
    // alert error if memory allocation fails.
    if (!itemTAIL) {
        fprintf(stderr, "[ERROR] Out of memory.\n");
        return 0;
    }
    // set tail pointer 0 to link to new data chunk
    itemTAIL->link = NULL;
    // input the integer
    itemTAIL->data = data;
    if (!(*head_pointer)) {
    // if data chunk is one, head pointer points to first data chuck
        *head_pointer = itemTAIL;
    } else {
    // the second data chunk from behind points to new data chuck
        (*tail_pointer)->link = itemTAIL;
    }
    // tail pointer points to the last data chunk
    *tail_pointer = itemTAIL;
    return 1;
}

/**
 *   Given head and tail of the linked list and data to delete,
 *   delete data in the linked list.
 *   Delete data into the second to first of list.
 */

int secondDelete(NODE **head_pointer, NODE **tail_pointer, int *data) {
    // set temporary data to ready to delete data
    NODE *temp = *head_pointer;
    // if deleting data finishes, terminate this function
    if (!temp) {
        return 0;
    }
    // set head data to ready to delete data
    NODE *itemHEAD = (*head_pointer)->link;
    // if data chunk is one, run below things
    if (!itemHEAD) {
        *data = temp->data;
        myfree(temp);
        *head_pointer = *tail_pointer = 0;
        return 1;
    }
    // a data chunk pointer points to subsequent data chunk
    *data = itemHEAD->data;
    // link the data chunk and subsequent data chunk twice skipped
    ((*head_pointer)->link) = (itemHEAD->link);
    // if data chunks are two, run below things
    if (!((*head_pointer)->link)) {
        *tail_pointer = *head_pointer;
    }
    // free the memory chunk
    myfree(itemHEAD);
    return 1;
}

/**
 *    Print a list from head to tail
 */
static void printList(NODE *head) {
    NODE *node = head;

    while (node) {
        printf(" %d", node->data);
        node = node->link;
    }
}

/**
 *   Given head and tail of the linked list, delete the linked list.
 *   Delete all of data in the linked list and free memory of the list.
 */

void deleteList(NODE **head_pointer, NODE **tail_pointer) {
    // repeat 'headDelete' function
    NODE *itemHEAD = *head_pointer;
    if (!itemHEAD) {
        fprintf(stderr, "[ERROR] There is no linked list.\n");
    } else {
        while (*head_pointer) {
            *head_pointer = itemHEAD->link;
            myfree(itemHEAD);
            itemHEAD = *head_pointer;
        }
        *head_pointer = *tail_pointer = NULL;
    }
}

/* linkedlist.c */

